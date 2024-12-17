// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/PanGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbility/AbilityTask/PanAT_Trace.h"
#include "GameplayAbility/TargetActor/PanTA_Trace.h"
#include "Attribute/PanCharacterAttributeSet.h"
#include "Tag/PanGameplayTag.h"

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

	// 전송받은 데이터의 값을 [현재 레벨(콤보 공격 횟수)]로 설정
	CurrentLevel = TriggerEventData->EventMagnitude;

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
		// 소스 어트리뷰트로 정보를 가져온다.
		const UPanCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UPanCharacterAttributeSet>();
		// 피격 결과로 부터 타겟 ASC 가져오기
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

		// BPGE_AttackDamage(게임 이펙트 블루프린트)에서 아래 로직을 구현하기 때문에 주석처리
		//// 소스ASC와 타겟ASC 둘중 하나만 없어도 로직스킵
		//if (!SourceASC || !TargetASC)
		//{
		//	PAN_LOG(LogGAS, Error, TEXT("ASC not found!"));
		//	return;
		//}
		//// 타겟 어트리뷰트는 값을 변경해야 하기 때문에 const_cast로 const를 없앤다
		//UPanCharacterAttributeSet* TargetAttribute = const_cast<UPanCharacterAttributeSet*>(TargetASC->GetSet<UPanCharacterAttributeSet>());
		//// 소스 어트리뷰트와 타겟 어트리뷰트 둘중 하나만 없어도 로직스킵
		//if (!SourceAttribute || !TargetAttribute)
		//{
		//	PAN_LOG(LogGAS, Error, TEXT("ASC not found!"));
		//	return;
		//}
		//// 공격자의 공격력을 데미지로 설정
		//const float AttackDamage = SourceAttribute->GetAttackRate();
		//// 피격자의 체력에서 데미지만큼 차감
		//TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);

		// 레벨에 따른 데미지 GE스펙을 생성
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		// GE스펙핸들이 유효하다면
		if (EffectSpecHandle.IsValid())
		{
			//// BPGE_AttackDamage(게임 이펙트 블루프린트)에서 SetByCaller 옵션을 통해 태그에 속성값 전달
			//EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());
			// 지정된 GE스펙을 타겟액터의 ASC에 적용
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		
			// 이펙트 컨텍스트 생성 (추가적인 정보를 담는 그릇)
			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			// 이펙트 컨텍스트에 충돌 결과 추가
			CueContextHandle.AddHitResult(HitResult);
			// GC 파라미터 선언
			FGameplayCueParameters CueParam;
			// GC 파라미터에 이펙트 컨텍스트 설정
			CueParam.EffectContext = CueContextHandle;

			// 태그를 통해 GC 실행
			TargetASC->ExecuteGameplayCue(TAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}

		// 버프 이펙트 GE스펙을 생성
		FGameplayEffectSpecHandle BuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect);
		// GE스펙핸들이 유효하다면
		if (BuffEffectSpecHandle.IsValid())
		{
			// 지정된 GE스펙을 자신에게 적용
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffEffectSpecHandle);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	// 어빌리티 종료
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
