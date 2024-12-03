// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/TargetActor/PanTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/PanCollision.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PanCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"

APanTA_Trace::APanTA_Trace()
{
}

/*************************************************************************************************
 * 타겟팅 시작
 *
 * @author	조현식
 * @date	2024/11/29
 * @param	어빌리티
 **************************************************************************************************/
void APanTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	// 어빌리티로부터 액터를 얻음
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

/*************************************************************************************************
 * 타겟팅을 확정하고 이후 남은 프로세스를 진행
 *
 * @author	조현식
 * @date	2024/11/29
 **************************************************************************************************/
void APanTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		// 어빌리티 타겟 데이터 핸들 생성
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		// 델리게이트 실행
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

/*************************************************************************************************
 * 충돌을 바탕으로 타겟 데이터 생성
 *
 * @author	조현식
 * @date	2024/11/29
 * @return	타겟 데이터 핸들
 **************************************************************************************************/
FGameplayAbilityTargetDataHandle APanTA_Trace::MakeTargetData() const
{
	// 캐릭터로 변환
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	// ASC가 없을 때 로직스킵
	if (!ASC)
	{
		return FGameplayAbilityTargetDataHandle();
	}
	const UPanCharacterAttributeSet* AttributeSet = ASC->GetSet<UPanCharacterAttributeSet>();
	// 어트리뷰트 세트가 없을 때 로직스킵
	if (!AttributeSet)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	// 피격결과
	FHitResult OutHitResult;
	// 범위
	const float AttackRange = AttributeSet->GetAttackRange();
	// 반지름
	const float AttackRadius = AttributeSet->GetAttackRadius();
	// 충돌 쿼리 파라미터
	FCollisionQueryParams Params(
		/*성능 프로파일링*/SCENE_QUERY_STAT(UABTA_Trace), 
		/*단순히트체크*/false, 
		/*자기자신은 무시*/Character);
	// 전방벡터
	const FVector Forward = Character->GetActorForwardVector();
	// 시작지점
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	// 끝지점
	const FVector End = Start + Forward * AttackRange;
	// 충돌 검출 (처리방법 : Sweep, 대상 : Single, 처리설정 : ByChannel)
	bool HitDetected = GetWorld()->SweepSingleByChannel(
		/*스윕 과정에서 첫번째 충돌 정보*/OutHitResult, 
		/*스윕 시작 위치*/Start, 
		/*스윕 종료 위치*/End, 
		/*스윕할 모양의 회전*/FQuat::Identity,
		/*트레이스 채널 : 어떤 종류의 오브젝트와 충돌할 지 결정*/CCHANNEL_PAN_ACTION,
		/*스윕할 모양*/FCollisionShape::MakeSphere(AttackRadius),
		/*충돌 검사의 추가 옵션*/Params);
	// 타겟 데이터 핸들
	FGameplayAbilityTargetDataHandle DataHandle;
	// 충돌이 발생했을 때
	if (HitDetected)
	{
		// 피격 결과로 부터 타겟 데이터 생성
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		// 핸들에 타겟 데이터 추가
		DataHandle.Add(TargetData);
	}

// 드로우 디버그 매크로가 활성화 상태라면
#if ENABLE_DRAW_DEBUG
	// 충돌 디버깅을 허용했다면
	if (bShowDebug)
	{
		// 캡슐 중심
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		// 캡슐 범위
		float CapsuleHalfHeight = AttackRange * 0.5f;
		// 충돌 발생했는 지를 색으로 표현
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		// 디버그 캡슐 그리기
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
	}
#endif
	return DataHandle;
}
