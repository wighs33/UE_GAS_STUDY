// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/AbilityTask/PanAT_Trace.h"
#include "GameplayAbility/TargetActor/PanTA_Trace.h"
#include "AbilitySystemComponent.h"

UPanAT_Trace::UPanAT_Trace()
{
}

/*************************************************************************************************
 * 태스크 생성
 *
 * @author	조현식
 * @date	2024/11/30
 * @param	소유어빌리티, 타겟액터클래스
 * @return	태스크
 **************************************************************************************************/
UPanAT_Trace* UPanAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<APanTA_Trace> TargetActorClass)
{
	// 어빌리티 태스크 인스턴스 생성
	UPanAT_Trace* NewTask = NewAbilityTask<UPanAT_Trace>(OwningAbility);
	// 태스크에 타겟액터클래스 할당
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

/*************************************************************************************************
 * 태스크 시작
 *
 * @author	조현식
 * @date	2024/11/30
 **************************************************************************************************/
void UPanAT_Trace::Activate()
{
	Super::Activate();
	// 메서드 : 타겟팅 액터를 스폰하고 초기화
	SpawnAndInitializeTargetActor();
	// 메서드 : 타겟 액터 마무리
	FinalizeTargetActor();
	// 아바타에 의존적인 작업이 필요하므로 대기 설정
	SetWaitingOnAvatar();
}

/*************************************************************************************************
 * 삭제될 때 호출
 *
 * @author	조현식
 * @date	2024/11/30
 * @param	어빌리티종료
 **************************************************************************************************/
void UPanAT_Trace::OnDestroy(bool AbilityEnded)
{
	// 타겟 액터가 존재한다면
	if (SpawnedTargetActor)
	{
		// 타겟 액터 삭제
		SpawnedTargetActor->Destroy();
	}
	Super::OnDestroy(AbilityEnded);
}

/*************************************************************************************************
 * 타겟팅 액터를 스폰하고 초기화
 *
 * @author	조현식
 * @date	2024/11/30
 **************************************************************************************************/
void UPanAT_Trace::SpawnAndInitializeTargetActor()
{
	// 지정된 클래스의 액터를 스폰하지만, 초기화 과정은 지연시킨다
	SpawnedTargetActor = Cast<APanTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(
		/*스폰할 액터 클래스*/TargetActorClass,
		/*액터의 트랜스폼*/FTransform::Identity,
		/*스폰된 액터의 소유자*/nullptr,
		/*액터를 스폰한 주체*/nullptr,
		/*스폰 시 충돌 처리 방식*/ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

	// 타겟 액터가 존재한다면
	if (SpawnedTargetActor)
	{
		// 타겟 액터의 충돌 디버깅 활성화
		SpawnedTargetActor->SetShowDebug(true);
		// 타겟 액터의 데이터준비델리게이트에 콜백함수 바인딩
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UPanAT_Trace::OnTargetDataReadyCallback);
	}
}

/*************************************************************************************************
 * 타겟 액터 마무리
 *
 * @author	조현식
 * @date	2024/11/30
 **************************************************************************************************/
void UPanAT_Trace::FinalizeTargetActor()
{
	// 어빌리티 시스템 컴포넌트가 있다면
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		// 아바타의 트랜스폼 가져오기
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		// 지연시켰던 초기화 완료, 아바타의 트랜스폼으로 설정
		SpawnedTargetActor->FinishSpawning(SpawnTransform);
		// ASC의 타겟 액터 그룹에 타겟 액터 추가
		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		// 타겟팅 시작
		SpawnedTargetActor->StartTargeting(Ability);
		// 타겟팅을 확정하고 이후 남은 프로세스 진행
		SpawnedTargetActor->ConfirmTargeting();
	}
}

/*************************************************************************************************
 * 타겟 데이터가 준비되었다고 구독자들에게 알림
 *
 * @author	조현식
 * @date	2024/11/30
 * @param	타겟 데이터 핸들
 **************************************************************************************************/
void UPanAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	// 현재 설정이 알림 가능한 지 파악
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// 구독자에게 알리기
		OnComplete.Broadcast(DataHandle);
	}
	// 태스크 종료
	EndTask();
}