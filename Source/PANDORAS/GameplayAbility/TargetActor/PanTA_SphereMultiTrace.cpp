// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/TargetActor/PanTA_SphereMultiTrace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Physics/PanCollision.h"
#include "DrawDebugHelpers.h"
//#include "Attribute/PanCharacterSkillAttributeSet.h"
#include "Engine/OverlapResult.h"

#include "PANDORAS.h"

FGameplayAbilityTargetDataHandle APanTA_SphereMultiTrace::MakeTargetData() const
{
	// 원본액터를 캐릭터로 변환
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	// 원본액터에서 ASC 못 가져오면 로직스킵
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		PAN_LOG(LogGAS, Error, TEXT("ASC not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	// ASC로부터 [스킬 어트리뷰트 세트] 못 가져오면 로직스킵
	//const UABCharacterSkillAttributeSet* SkillAttribute = ASC->GetSet<UABCharacterSkillAttributeSet>();
	//if (!SkillAttribute)
	//{
	//	Pan_LOG(LogPan, Error, TEXT("SkillAttribute not found!"));
	//	return FGameplayAbilityTargetDataHandle();
	//}

	// 오버랩 결과 배열
	TArray<FOverlapResult> Overlaps;
	// 어트리뷰트에서 스킬범위 얻기
	//const float SkillRadius = SkillAttribute->GetSkillRange();
	const float SkillRadius = 800.0f;
	// 캐릭터 위치
	FVector Origin = Character->GetActorLocation();
	// 충돌 쿼리 파라미터
	FCollisionQueryParams Params(
		/*성능 프로파일링*/SCENE_QUERY_STAT(APanTA_SphereMultiTrace),
		/*단순히트체크*/false,
		/*자기자신은 무시*/Character);
	// 충돌 검출 (처리방법 : Overlap, 대상 : Multi, 처리설정 : ByChannel)
	GetWorld()->OverlapMultiByChannel(
		/*오버랩 과정에서 검출한 정보*/Overlaps,
		/*오버랩 중심 위치*/Origin,
		/*오버랩할 모양의 회전*/FQuat::Identity,
		/*트레이스 채널 : 어떤 종류의 오브젝트와 충돌할 지 결정*/CCHANNEL_PAN_ACTION,
		/*오버랩할 모양*/FCollisionShape::MakeSphere(SkillRadius),
		/*충돌 검사의 추가 옵션*/Params);
	
	// 약참조 액터 배열 (SetActors 함수의 인수가 이런 형식이기 때문에 똑같이 만들기)
	TArray<TWeakObjectPtr<AActor>> HitActors;
	// 오버랩 정보 하나씩
	for (const FOverlapResult& Overlap : Overlaps)
	{
		// 오버랩된 오브젝트를 AActor* 타입으로 가져오기
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		// 액터가 유효하고 동시에 액터가 배열에 포함되있지 않다면
		if (HitActor && !HitActors.Contains(HitActor))
		{
			// 배열에 액터 추가
			HitActors.Add(HitActor);
		}
	}

	// 액터 배열을 타겟 데이터로 관리하기 위해 선언
	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	// 수집한 액터목록을 타겟 데이터로 설정
	ActorsData->SetActors(HitActors);

// 드로우 디버그 매크로가 활성화 상태라면
#if ENABLE_DRAW_DEBUG
	// 충돌 디버깅을 허용했다면
	if (bShowDebug)
	{
		// 충돌 발생했는 지를 색으로 표현
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		// 디버그 구체 그리기
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.0f);
	}
#endif

	//타겟 데이터를 FGameplayAbilityTargetDataHandle로 매핑한 뒤 반환
	return FGameplayAbilityTargetDataHandle(ActorsData);
}
