// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/PanGA_Rotate.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Tag/PanGameplayTag.h"

UPanGA_Rotate::UPanGA_Rotate()
{
	// 행동 태그 지정
	AbilityTags.AddTag(TAG_ACTOR_ROTATE);
	// 상태 태그 지정
	ActivationOwnedTags.AddTag(TAG_ACTOR_ISROTATING);
}

/*************************************************************************************************
 * 어빌리티 발동 시 호출
 *
 * @author	조현식
 * @date	2024/10/13
 * @param	Handle, ActorInfo, ActivationInfo, TriggerEventData
 **************************************************************************************************/
void UPanGA_Rotate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 아바타 액터 사용
	if (AActor* AvatarActor = ActorInfo->AvatarActor.Get())
	{
		// 클래스 이름으로 회전 컴포넌트 가져오기
		if (URotatingMovementComponent* RotatingMovement = Cast<URotatingMovementComponent>(AvatarActor->GetComponentByClass(URotatingMovementComponent::StaticClass())))
		{
			// 활성화
			RotatingMovement->Activate(true);
		}
	}
}

/*************************************************************************************************
 * 어빌리티 취소 시 호출
 *
 * @author	조현식
 * @date	2024/10/13
 * @param	Handle, ActorInfo, ActivationInfo, TriggerEventData
 **************************************************************************************************/
void UPanGA_Rotate::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	// 아바타 액터 사용
	if (AActor* AvatarActor = ActorInfo->AvatarActor.Get())
	{
		// 클래스 이름으로 회전 컴포넌트 가져오기
		if (URotatingMovementComponent* RotatingMovement = Cast<URotatingMovementComponent>(AvatarActor->GetComponentByClass(URotatingMovementComponent::StaticClass())))
		{
			// 비활성화
			RotatingMovement->Deactivate();
		}
	}
}
