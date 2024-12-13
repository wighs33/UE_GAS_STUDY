// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/PanCharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Tag/PanGameplayTag.h"

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
 * GE를 통해 어트리뷰트 값 변경되기 직전에 호출됨
 *
 * @author	조현식
 * @date	2024/12/05
 * @param	GE 데이터
 **************************************************************************************************/
bool UPanCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	// 호출 실패 시 로직 스킵
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	// GE로 들어온 값의 어트리뷰트가 데미지라면 
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// 평가된 데미지 값이 양수라면
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			//대상이 무적 상태라면
			if (Data.Target.HasMatchingGameplayTag(TAG_CHARACTER_INVINSIBLE))
			{
				// 데미지를 0으로 설정하고 값 변경 실패 반환
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}

	return true;
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

	// 체력이 0이하인 동시에 체력고갈플래그가 false일 때
	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		// 대상의 ASC에 캐릭터 사망 태그를 일시적으로 추가하여 캐릭터가 사망한 상태임을 표시
		Data.Target.AddLooseGameplayTag(TAG_CHARACTER_ISDEAD);
		// 체력고갈시점에 바인딩한 함수들에게 알림
		OnOutOfHealth.Broadcast();
	}
	// 체력고갈플래그를 갱신
	bOutOfHealth = (GetHealth() <= 0.0f);
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