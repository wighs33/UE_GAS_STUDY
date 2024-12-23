// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffect/PanSkillDamageExecutionCalc.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PanCharacterSkillAttributeSet.h"
#include "Attribute/PanCharacterAttributeSet.h"

void UPanSkillDamageExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	// 소스ASC와 타겟ASC 가져오는 데 성공했다면
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (SourceASC && TargetASC)
	{
		// 소스 액터와 타겟 액터 가져오는 데 성공했다면
		AActor* SourceActor = SourceASC->GetAvatarActor();
		AActor* TargetActor = TargetASC->GetAvatarActor();
		if (SourceActor && TargetActor)
		{
			// 어트리뷰트에서 최대 대미지와 그 범위 가져오기
			const float MaxDamageRange = SourceASC->GetNumericAttributeBase(UPanCharacterSkillAttributeSet::GetSkillRangeAttribute());
			const float MaxDamage = SourceASC->GetNumericAttributeBase(UPanCharacterSkillAttributeSet::GetSkillAttackRateAttribute());
			// 소스 액터와 타겟 액터 거리를 0과 최대 데미지 범위 사이로 클램핑
			const float Distance = FMath::Clamp(SourceActor->GetDistanceTo(TargetActor), 0.0f, MaxDamageRange);
			// 인버스화 : 거리가 멀어질 수록 [데미지 관련 수치] 감소시키기
			const float InvDamageRatio = 1.0f - Distance / MaxDamageRange;
			// 최대 데미지에 [데미지 관련 수치]를 곱해서 현재 데미지로 적용
			float Damage = InvDamageRatio * MaxDamage;
			// OutExecutionOutput에 모디파이어 값 전달
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UPanCharacterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
		}
	}
}
