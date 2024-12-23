// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/PanGA_Skill.h"
#include "Character/PanCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UPanGA_Skill::UPanGA_Skill()
{
}

/*************************************************************************************************
 * 어빌리티 발동 시 호출
 *
 * @author	조현식
 * @date	2024/12/20
 * @param	
 **************************************************************************************************/
void UPanGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// 쿨다운이나 코스트를 적용하기 위해 호출
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	// 타겟 캐릭터 있는 지 확인
	APanCharacterPlayer* TargetCharacter = Cast<APanCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (!TargetCharacter)
	{
		return;
	}

	// 스킬액션 몽타주 있는 지 확인
	ActiveSkillActionMontage = TargetCharacter->GetSkillActionMontage();
	if (!ActiveSkillActionMontage)
	{
		return;
	}

	// 이동 불가 설정
	TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	// 스킬 태스크 생성
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		/*소유 어빌리티*/this,
		/*태스크 이름*/TEXT("SkillMontage"),
		/*몽타주 애셋*/ActiveSkillActionMontage,
		/*속도*/1.0f);
	// 태스크가 끝나는 시점 바인딩
	PlayMontageTask->OnCompleted.AddDynamic(this, &UPanGA_Skill::OnCompleteCallback);
	// 태스크가 취소된 시점 바인딩
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UPanGA_Skill::OnInterruptedCallback);
	// 태스크 실행 준비
	PlayMontageTask->ReadyForActivation();
}

/*************************************************************************************************
 * 어빌리티 종료 시 호출
 *
 * @author	조현식
 * @date	2024/12/20
 * @param	
 **************************************************************************************************/
void UPanGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 타겟 캐릭터 있는 지 확인
	APanCharacterPlayer* TargetCharacter = Cast<APanCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (TargetCharacter)
	{
		// 캐릭터 움직임을 걷기 상태로 변경
		TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/*************************************************************************************************
 * 태스크 완료 시 호출
 *
 * @author	조현식
 * @date	2024/12/20
 * @param	
 **************************************************************************************************/
void UPanGA_Skill::OnCompleteCallback()
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
 * @date	2024/12/20
 * @param	
 **************************************************************************************************/
void UPanGA_Skill::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	// 어빌리티가 취소되었다고 지정
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
