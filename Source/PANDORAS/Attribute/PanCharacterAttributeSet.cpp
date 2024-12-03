// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/PanCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"

#include "PANDORAS.h"

UPanCharacterAttributeSet::UPanCharacterAttributeSet()
{
	// 현재 체력과 최대 체력 같은 값으로 초기화
	InitHealth(GetMaxHealth());
}

/*************************************************************************************************
 * 어트리뷰트 값 변경되기 직전에 호출됨
 *
 * @author	조현식
 * @date	2024/12/01
 * @param	어트리뷰트, 값
 **************************************************************************************************/
void UPanCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//// 어트리뷰트가 체력이면 
	//if (Attribute == GetHealthAttribute())
	//{
	//	// 값의 0과 최대 체력 사이로 클램핑한다.
	//	NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	//}

	// 어트리뷰트가 데미지이면
	if (Attribute == GetDamageAttribute())
	{
		// 값이 음수일 때는 0으로 변경
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

/*************************************************************************************************
 * GE를 통해 어트리뷰트 값 변경된 직후에 호출됨
 *
 * @author	조현식
 * @date	2024/12/04
 * @param	GE 데이터
 **************************************************************************************************/
void UPanCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 최소 체력은 0
	float MinimumHealth = 0.0f;
	// GE로 들어온 값의 어트리뷰트가 체력이면 
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		PAN_LOG(LogGAS, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		// 현재 체력을 최소 체력과 최대 체력 사이로 클램핑
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	// GE로 들어온 값의 어트리뷰트가 데미지이면 
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		PAN_LOG(LogGAS, Log, TEXT("Damage : %f"), GetDamage());
		// [현재 체력 - 데미지]를 최소 체력과 최대 체력 사이로 클램핑
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		// 데미지는 적용 후 0으로 초기화
		SetDamage(0.0f);
	}
}

/*************************************************************************************************
 * 어트리뷰트 값 변경된 직후에 호출됨
 *
 * @author	조현식
 * @date	2024/12/01
 * @param	
 * @return	
 **************************************************************************************************/
//void UPanCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
//{
//	// 어트리뷰트가 체력이면
//	if (Attribute == GetHealthAttribute())
//	{
//		// 체력 변화 로그
//		PAN_LOG(LogGAS, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
//	}
//}