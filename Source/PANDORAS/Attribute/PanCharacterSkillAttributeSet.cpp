// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/PanCharacterSkillAttributeSet.h"

UPanCharacterSkillAttributeSet::UPanCharacterSkillAttributeSet()
{
}

/*************************************************************************************************
 * 어트리뷰트 값 변경되기 직전에 호출됨
 *
 * @author	조현식
 * @date	2024/12/21
 * @param	어트리뷰트, 값
 **************************************************************************************************/
void UPanCharacterSkillAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	// 어트리뷰트가 스킬 범위라면
	if (Attribute == GetSkillRangeAttribute())
	{
		//  0.1과 최대 스킬 범위 사이로 클램핑
		NewValue = FMath::Clamp(NewValue, 0.1f, GetMaxSkillRange());
	}
	// 어트리뷰트가 스킬 공격력이라면
	else if (Attribute == GetSkillAttackRateAttribute())
	{
		//  0과 최대 스킬 공격력 사이로 클램핑
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSkillAttackRate());
	}
}