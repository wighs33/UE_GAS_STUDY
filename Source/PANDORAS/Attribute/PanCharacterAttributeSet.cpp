// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/PanCharacterAttributeSet.h"
#include "PANDORAS.h"

UPanCharacterAttributeSet::UPanCharacterAttributeSet() :
	AttackRange(100.0f),
	AttackRadius(50.f),
	AttackRate(30.0f),
	MaxAttackRange(300.0f),
	MaxAttackRadius(150.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f)
{
	// 현재 체력과 최대 체력 같은 값으로 초기화
	InitHealth(GetMaxHealth());
}

/*************************************************************************************************
 * 어트리뷰트 값 변경되기 직전에 호출됨
 *
 * @author	조현식
 * @date	2024/12/01
 * @param	
 * @return	
 **************************************************************************************************/
void UPanCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 어트리뷰트가 체력이면 
	if (Attribute == GetHealthAttribute())
	{
		// 값의 0과 최대 체력 사이로 클램핑한다.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
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
void UPanCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	// 어트리뷰트가 체력이면
	if (Attribute == GetHealthAttribute())
	{
		// 체력 변화 로그
		PAN_LOG(LogGAS, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
	}
}