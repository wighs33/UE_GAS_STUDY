 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\GameplayAbility\PanGA_Jump.h
 * 
 * #include "GameplayAbility/PanGA_Jump.h"
 * 
 * 점프 키 누른 후 땅에 닿기까지 점프 상태를 유지하기 위해 커스텀 점프 어빌리티 구현
 * PanAT_JumpAndWaitForLanding에서 태스크 수행
 *
 * <BPGA_Jump>
 * Owned Tag : Character.State.IsJumping
 * Blocked Tag :
 * Character.State.IsAttacking
 * Character.State.IsDead
 * Character.State.IsSkilling
 *
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PanGA_Jump.generated.h"

UCLASS()
class PANDORAS_API UPanGA_Jump : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPanGA_Jump();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

//protected:
//	UFUNCTION()
//	void OnLandedCallback();
	
};
