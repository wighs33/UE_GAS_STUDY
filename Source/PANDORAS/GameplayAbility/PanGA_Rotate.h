// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PanGA_Rotate.generated.h"

/*************************************************************************************************
 * 회전 기능의 어빌리티 (URotatingMovementComponent 역할을 대신함)
 * 분수대 액터의 [시작 어빌리티 그룹]에 추가
 * 
 * @author	조현식
 * @date	2024/10/13
 **************************************************************************************************/
UCLASS()
class PANDORAS_API UPanGA_Rotate : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPanGA_Rotate();

// 게임 어빌리티 동작
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
};
