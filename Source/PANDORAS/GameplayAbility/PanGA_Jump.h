// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PanGA_Jump.generated.h"

/*************************************************************************************************
 * 점프 키 누른 후 땅에 닿기까지 점프 상태를 유지하기 위해 커스텀 점프 어빌리티 구현
 *
 * @author	조현식
 * @date	2024/10/16
 * @param	
 * @return	
 **************************************************************************************************/
UCLASS()
class PANDORAS_API UPanGA_Jump : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPanGA_Jump();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UFUNCTION()
	void OnLandedCallback();
	
};
