// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffect/PanGE_AttackDamage.h"
#include "Attribute/PanCharacterAttributeSet.h"

UPanGE_AttackDamage::UPanGE_AttackDamage()
{
	// 지속 옵션 (Instant : 한 프레임에 바로 어트리뷰트에 즉각적으로 적용)
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// 체력 모디파이어 (체력 변경 방법 지정)
	FGameplayModifierInfo HealthModifier;
	// 변경할 어트리뷰트 값을 지정해준다.
	HealthModifier.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(
		/*속성을 찾을 클래스*/UPanCharacterAttributeSet::StaticClass(), 
		/*이름을 안전하게 가져오기*/GET_MEMBER_NAME_CHECKED(UPanCharacterAttributeSet, Health)));
	// 변경할 연산방법을 '더하기'로 지정
	HealthModifier.ModifierOp = EGameplayModOp::Additive;

	// 데미지 값
	FScalableFloat DamageAmount(-30.0f);
	// 실제로 적용할 값 지정
	FGameplayEffectModifierMagnitude ModMagnitude(DamageAmount);

	// 기존 체력 정보에 데미지 값을 더함
	HealthModifier.ModifierMagnitude = ModMagnitude;
	
	// 모디파이어 그룹에 추가
	Modifiers.Add(HealthModifier);
}
