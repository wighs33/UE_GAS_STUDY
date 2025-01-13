// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility/PanGA_Attack.h"
#include "Character/PanCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PanComboActionData.h"

UPanGA_Attack::UPanGA_Attack()
{
	// 인스턴스 옵션 (InstancedPerActor : 액터마다 하나의 인스턴스 생성)
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




	// 단일 공격 테스트
	//CurrentComboData = PanCharacter->GetComboActionData();
	



	// 캐릭터의 이동을 없애기
	PanCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 공격 태스크 생성 (공격 몽타주 재생, Proxy : AT와 상호작용할 수 있는 중개자)
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		/*소유 어빌리티*/this, 
		/*태스크 이름*/TEXT("PlayAttack"), 
		/*몽타주 애셋*/PanCharacter->GetComboActionMontage(),
		/*속도*/1.f, 
		/*시작섹션*/GetNextSection());
	// 태스크가 끝나는 시점 바인딩
	PlayAttackTask->OnCompleted.AddDynamic(this, &UPanGA_Attack::OnCompleteCallback);
	// 태스크가 취소된 시점 바인딩
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UPanGA_Attack::OnInterruptedCallback);
	// 태스크 실행 준비
	PlayAttackTask->ReadyForActivation();




	// 단일 공격 테스트
	// 시작 콤보 타이머
	//StartComboTimer();
}

/*************************************************************************************************
 * 어빌리티 발동 후 입력이 있을 때 호출
 *
 * @author	조현식
 * @date	2024/11/18
 * @param	Handle, ActorInfo,...
 **************************************************************************************************/
void UPanGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// 콤보 타이머 핸들이 유효한지 체크
	if (!ComboTimerHandle.IsValid())
	{
		// 다음 콤보는 없다고 지정
		HasNextComboInput = false;
	}
	else
	{
		// 다음 콤보가 있다고 지정
		HasNextComboInput = true;
	}
}

/*************************************************************************************************
 * 어빌리티 취소 시 호출 
 *
 * @author	조현식
 * @date	2024/11/22
 * @param	Handle, ActorInfo,...
 **************************************************************************************************/
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

	// 콤보 데이터 초기화
	CurrentComboData = nullptr;
	// 콤보 횟수 0 초기화
	CurrentCombo = 0;
	// 다음 콤보는 없다고 지정
	HasNextComboInput = false;
}

/*************************************************************************************************
 * 태스크 완료 시 호출
 *
 * @author	조현식
 * @date	2024/11/18
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
 **************************************************************************************************/
void UPanGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	// 어빌리티가 취소되었다고 지정
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

/*************************************************************************************************
 * 다음 섹션 이름 얻기
 *
 * @author	조현식
 * @date	2024/11/22
 * @return	FName
 **************************************************************************************************/
FName UPanGA_Attack::GetNextSection()
{
	// [현재 콤보 횟수 + 1]이 [최대 콤보 횟수] 내에 있는 지 검사
	//CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	// NextSection = [몽타주이름][콤보횟수]
	//FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);
	//return NextSection;






	// 단일 공격 테스트
	return *FString::Printf(TEXT("aa"));
}

/*************************************************************************************************
 * 콤보 체크 타이머 시작
 *
 * @author	조현식
 * @date	2024/11/22
 **************************************************************************************************/
void UPanGA_Attack::StartComboTimer()
{
	// 현재 콤보 레벨 - 1 = 콤보 인덱스
	int32 ComboIndex = CurrentCombo - 1;
	// EffectiveFrameCount배열에서 ComboIndex가 유효한 인덱스인지 확인
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));
	// 현재 콤보 레벨의 프레임 수 / fps = 콤보 시간
	const float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	// 콤보 시간이 0보다 큰 지 체크
	if (ComboEffectiveTime > 0.f)
	{
		// 콤보 레벨마다 콤보 시간동안 콤보 진행되었는 지 확인
		GetWorld()->GetTimerManager().SetTimer(
			/*핸들*/ComboTimerHandle,
			/*대상*/this,
			/*적용함수*/&UPanGA_Attack::CheckComboInput,
			/*주기*/ComboEffectiveTime,
			/*루프*/false);
	}
}

/*************************************************************************************************
 * 콤보 체크
 * StartComboTimer함수 내부 타이머마다 호출
 *
 * @author	조현식
 * @date	2024/11/22
 **************************************************************************************************/
void UPanGA_Attack::CheckComboInput()
{
	// 콤보 타이머 핸들 초기화
	ComboTimerHandle.Invalidate();
	// 다음 콤보 커맨드 여부 체크
	if (HasNextComboInput)
	{
		// 앞서만든 이름으로 찾아서 다음 몽타주 섹션으로 건너뛰기
		MontageJumpToSection(GetNextSection());
		// 콤보 체크 타이머 재시작
		StartComboTimer();
		// 다음 콤보는 없다고 지정
		HasNextComboInput = false;
	}
}