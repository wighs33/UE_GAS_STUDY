 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\GameplayAbility\PanGA_Attack.h
 * 
 * #include "GameplayAbility/PanGA_Attack.h"
 * 
 * 콤보 공격 기능
 * 
 * <BPGA_Jump>
 * Owned Tag : Character.State.IsAttacking
 * Blocked Tag :
 * Character.State.IsJumping
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
#include "PanGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UPanGA_Attack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPanGA_Attack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();

	FName GetNextSection();
	void StartComboTimer();
	void CheckComboInput();

protected:
	UPROPERTY()
	TObjectPtr<class UPanComboActionData> CurrentComboData;
	uint8 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboInput = false;
};
