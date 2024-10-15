// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/PanGA_Jump.h"
#include "GameFramework/Character.h"
#include "GameplayAbility/AbilityTask/PanAT_JumpAndWaitForLanding.h"

UPanGA_Jump::UPanGA_Jump()
{
	// 인스턴스 정책 (InstancedPerActor : 액터마다 하나의 인스턴스 생성)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

/*************************************************************************************************
 * 어빌리티가 발동될 수 있는 지 파악
 *
 * @author	조현식
 * @date	2024/10/16
 * @param	Handle, ActorInfo,...
 * @return	캐릭터가 점프 가능한 지
 **************************************************************************************************/
bool UPanGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// ActorInfo를 통해 캐릭터 얻기
	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	// 리턴값 : 캐릭터가 점프 가능한 지
	return (Character && Character->CanJump());
}

/*************************************************************************************************
 * 어빌리티 발동 시 호출
 *
 * @author	조현식
 * @date	2024/10/16
 * @param	Handle, ActorInfo,...
 **************************************************************************************************/
void UPanGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//// 블루프린트로 구현
	//{
	//	// 태스크 생성
	//	UPanAT_JumpAndWaitForLanding* JumpAndWaitingForLandingTask = UPanAT_JumpAndWaitForLanding::CreateTask(this);
	//	// 태스크의 착지 델리게이트에 함수 등록
	//	JumpAndWaitingForLandingTask->OnComplete.AddDynamic(this, &UPanGA_Jump::OnLandedCallback);
	//	// 태스크 활성화
	//	JumpAndWaitingForLandingTask->ReadyForActivation();
	//}
}

/*************************************************************************************************
 * 입력이 해제될 때 호출
 *
 * @author	조현식
 * @date	2024/10/16
 * @param	Handle, ActorInfo,...
 **************************************************************************************************/
void UPanGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// ActorInfo를 통해 캐릭터 얻기
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	// 캐릭터 점프 멈추기
	Character->StopJumping();
}

/*************************************************************************************************
 * 착지 알림 받을 함수
 *
 * @author	조현식
 * @date	2024/10/16
 **************************************************************************************************/
void UPanGA_Jump::OnLandedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;

	// 어빌리티 종료
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
