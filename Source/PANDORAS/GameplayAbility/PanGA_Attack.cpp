// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility/PanGA_Attack.h"
#include "Character/PanCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "Character/PanComboActionData.h"

UPanGA_Attack::UPanGA_Attack()
{
	// 인스턴스 정책 (InstancedPerActor : 액터마다 하나의 인스턴스 생성)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

/*************************************************************************************************
 * 어빌리티 발동 시 호출
 *
 * @author	조현식
 * @date	2024/10/16
 * @param	Handle, ActorInfo,...
 **************************************************************************************************/
void UPanGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// ActorInfo를 통해 캐릭터 얻기
	APanCharacterBase* PanCharacter = CastChecked<APanCharacterBase>(ActorInfo->AvatarActor.Get());
	// 캐릭터의 이동을 없애기
	PanCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 공격 태스크 생성
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		/*소유 어빌리티*/this, 
		/*태스크 이름*/TEXT("PlayAttack"), 
		/*몽타주 애셋*/PanCharacter->GetComboActionMontage());
	// 태스크가 끝나는 시점 바인딩
	PlayAttackTask->OnCompleted.AddDynamic(this, &UPanGA_Attack::OnCompleteCallback);
	// 태스크가 취소된 시점 바인딩
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UPanGA_Attack::OnInterruptedCallback);
	// 태스크 실행 준비
	PlayAttackTask->ReadyForActivation();
}

/*************************************************************************************************
 * 어빌리티 발동 후 입력이 있을 때 호출
 *
 * @author	조현식
 * @date	2024/11/18
 * @param	
 * @return	
 **************************************************************************************************/
void UPanGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UE_LOG(LogTemp, Log, TEXT("Attack"));
}

void UPanGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

/*************************************************************************************************
 * 어빌리티 종료 시 호출
 *
 * @author	조현식
 * @date	2024/10/16
 * @param	Handle, ActorInfo,...
 **************************************************************************************************/
void UPanGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// ActorInfo를 통해 캐릭터 얻기
	APanCharacterBase* ABCharacter = CastChecked<APanCharacterBase>(ActorInfo->AvatarActor.Get());
	// 캐릭터를 걷기 상태로 전환
	ABCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

/*************************************************************************************************
 * 태스크 완료 시 호출
 *
 * @author	조현식
 * @date	2024/11/18
 * @param	
 * @return	
 **************************************************************************************************/
void UPanGA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	// 어빌리티가 끝났다고 지정
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

/*************************************************************************************************
 * 태스크 취소 시 호출
 *
 * @author	조현식
 * @date	2024/11/18
 * @param	
 * @return	
 **************************************************************************************************/
void UPanGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	// 어빌리티가 취소되었다고 지정
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}