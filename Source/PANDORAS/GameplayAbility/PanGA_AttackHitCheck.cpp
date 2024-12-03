// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/PanGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbility/AbilityTask/PanAT_Trace.h"
#include "GameplayAbility/TargetActor/PanTA_Trace.h"
#include "Attribute/PanCharacterAttributeSet.h"

#include "PANDORAS.h"

UPanGA_AttackHitCheck::UPanGA_AttackHitCheck()
{
	// 인스턴스 옵션 (InstancedPerActor : 액터마다 하나의 인스턴스 생성)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

/*************************************************************************************************
 * 어빌리티 발동 시 호출
 *
 * @author	조현식
 * @date	2024/11/30
 * @param	
 * @return	
 **************************************************************************************************/
void UPanGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// 태스크 생성
	UPanAT_Trace* AttackTraceTask = UPanAT_Trace::CreateTask(this, APanTA_Trace::StaticClass());
	// 태스크의 트레이스 결과 델리게이트에 함수 등록
	AttackTraceTask->OnComplete.AddDynamic(this, &UPanGA_AttackHitCheck::OnTraceResultCallback);
	// 태스크 실행 준비
	AttackTraceTask->ReadyForActivation();
}

/*************************************************************************************************
 * 트레이스 결과 알림 받을 함수
 *
 * @author	조현식
 * @date	2024/11/30
 * @param	
 * @return	
 **************************************************************************************************/
void UPanGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 타겟데이터 그룹에 피격결과 있는지 조사
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, /*인덱스*/0))
	{
		// 타겟데이터 그룹 0번째의 피격결과 얻어냄
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		// ActorInfo로부터 ASC를 가져오고 불가능하면 에러를 발생
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		// 피격 결과로 부터 타겟 ASC 가져오기
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		// 소스ASC와 타겟ASC 둘중 하나만 없어도 로직스킵
		if (!SourceASC || !TargetASC)
		{
			PAN_LOG(LogGAS, Error, TEXT("ASC not found!"));
			return;
		}
		// 소스 어트리뷰트로 정보를 가져온다.
		const UPanCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UPanCharacterAttributeSet>();
		// 타겟 어트리뷰트는 값을 변경해야 하기 때문에 const_cast로 const를 없앤다
		UPanCharacterAttributeSet* TargetAttribute = const_cast<UPanCharacterAttributeSet*>(TargetASC->GetSet<UPanCharacterAttributeSet>());
		// 소스 어트리뷰트와 타겟 어트리뷰트 둘중 하나만 없어도 로직스킵
		if (!SourceAttribute || !TargetAttribute)
		{
			PAN_LOG(LogGAS, Error, TEXT("ASC not found!"));
			return;
		}
		// 공격자의 공격력을 데미지로 설정
		const float AttackDamage = SourceAttribute->GetAttackRate();
		// 피격자의 체력에서 데미지만큼 차감
		TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);

	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	// 어빌리티 종료
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
