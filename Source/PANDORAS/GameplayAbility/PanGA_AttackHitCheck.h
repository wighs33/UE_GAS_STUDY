 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\GameplayAbility\PanGA_AttackHitCheck.h
 *
 * #include "GameplayAbility/PanGA_AttackHitCheck.h"
 * 공격 탐지 어빌리티
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PanGA_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UPanGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPanGA_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
