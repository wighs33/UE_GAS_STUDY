// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/AbilityTask/PanAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UPanAT_JumpAndWaitForLanding::UPanAT_JumpAndWaitForLanding()
{
}

/*************************************************************************************************
 * 태스크 생성
 *
 * @author	조현식
 * @date	2024/10/16
 * @param	OwningAbility
 * @return	어빌리티 태스크 인스턴스
 **************************************************************************************************/
UPanAT_JumpAndWaitForLanding* UPanAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	// 어빌리티 태스크 인스턴스 생성
	UPanAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UPanAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

/*************************************************************************************************
 * 태스크 시작
 *
 * @author	조현식
 * @date	2024/10/16
 **************************************************************************************************/
void UPanAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	// 캐릭터 얻기
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	// 캐릭터가 땅에 착지할 때의 콜백함수 등록
	Character->LandedDelegate.AddDynamic(this, &UPanAT_JumpAndWaitForLanding::OnLandedCallback);
	// 캐릭터 점프
	Character->Jump();
	// 아바타에 의존적인 작업이 필요하므로 대기 설정
	SetWaitingOnAvatar();
}

/*************************************************************************************************
 * 태스크 종료 (어빌리티 종료 시 호출)
 *
 * @author	조현식
 * @date	2024/10/16
 **************************************************************************************************/
void UPanAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	// 캐릭터 얻기
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	// 캐릭터가 땅에 착지할 때의 콜백함수 등록해제
	Character->LandedDelegate.RemoveDynamic(this, &UPanAT_JumpAndWaitForLanding::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

/*************************************************************************************************
 * 땅에 착지했다고 구독자에게 알림
 *
 * @author	조현식
 * @date	2024/10/16
 * @param	
 * @return	
 **************************************************************************************************/
void UPanAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	// 현재 설정이 알림 가능한 지 파악
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// 구독자에게 알리기
		OnComplete.Broadcast();
	}
}
