// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PanItemBox.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/PanCollision.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interface/PanCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "PanItemData.h"

APanItemBox::APanItemBox()
{
	// 컴포넌트 생성
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// 종속 관계 설정
	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	// 트리거의 [충돌 프로필]을 [커스텀 트리거 프로필]로 설정
	Trigger->SetCollisionProfileName(CPROFILE_PAN_TRIGGER);
	// 트리거의 영역크기 설정
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	// 아이템상자 메시 애셋 로드
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	// 오브젝트가 유효하다면
	if (BoxMeshRef.Object)
	{
		// 메시 컴포넌트를 저장
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	// 상대 위치 설정
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	// 충돌 영역 없애기
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

/*************************************************************************************************
 * ASC 게터
 *
 * @author	조현식
 * @date	2024/12/16
 * @param	
 * @return	
 **************************************************************************************************/
UAbilitySystemComponent* APanItemBox::GetAbilitySystemComponent() const
{
	return ASC;
}

/*************************************************************************************************
 * 다른 액터와 오버랩 발생될 때 호출
 *
 * @author	조현식
 * @date	2024/12/16
 * @param	
 * @return	
 **************************************************************************************************/
void APanItemBox::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	// 액터가 플레이어 캐릭터라면
	IPanCharacterItemInterface* OverlappingPawn = Cast<IPanCharacterItemInterface>(Other);
	if (OverlappingPawn)
	{
		// 아이템 데이터를 플레이어 캐릭터에게 전달
		OverlappingPawn->SetItemData(Item);
	}

	// 오버랩이 발생한 액터를 대상으로 GC 실행
	InvokeGameplayCue(Other);
	// Other 액터에게 특정한 Gameplay Effect(능력 수치 변경, 버프/디버프 적용 등)를 적용하는 함수로 추정됩니다. 이를 통해 아이템 박스에 부딪힌 플레이어나 다른 캐릭터에게 특정 효과가 부여될 수 있습니다.
	ApplyEffectToTarget(Other);

	// 메시 숨기기
	Mesh->SetHiddenInGame(true);
	// 충돌 비활성화
	SetActorEnableCollision(false);
	// 2초 후 액터 제거
	SetLifeSpan(2.0f);

	// 액터의 ASC로 이벤트 전송
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Other, ItemEventTag, FGameplayEventData());
}

void APanItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// 어빌리티 등록
	ASC->InitAbilityActorInfo(this, this);

	// 프로젝트 세팅의 애셋 매니저
	UAssetManager& Manager = UAssetManager::Get();
	// 애셋 아이디 그룹
	TArray<FPrimaryAssetId> Assets;
	// [애셋 아이디 그룹]에 PanItemData 타입에 해당하는 모든 에셋의 PrimaryAssetId 채우기
	Manager.GetPrimaryAssetIdList(TEXT("PanItemData"), Assets);
	// [애셋 아이디 그룹]에 최소한 한 개의 애셋이 있다면
	ensure(0 < Assets.Num());
	// [애셋 아이디 그룹]의 수만큼 랜덤돌리기
	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	// 애셋 경로를 구해서 참조하기 (소프트 레퍼런싱 : 애셋 데이터가 필요한 시점에 로딩하는 방법)
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	// 로드되지 않았다면
	if (AssetPtr.IsPending())
	{
		// 동기방식으로 애셋을 로드
		AssetPtr.LoadSynchronous();
	}
	// 로드된 객체를 아이템 데이터로 변환
	Item = Cast<UPanItemData>(AssetPtr.Get());
	// 검사
	ensure(Item);
}

/*************************************************************************************************
 * 접근한 타겟에게 GE 적용
 *
 * @author	조현식
 * @date	2024/12/16
 * @param	
 * @return	
 **************************************************************************************************/
void APanItemBox::ApplyEffectToTarget(AActor* Target)
{
	// Target 액터에서 ASC 가져오는데 성공하면
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		// 해당 타겟에 영향을 줄 이펙트 컨텍스트 핸들을 생성
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		// 소스 오브젝트 정보로 컨텍스트 핸들구성
		EffectContext.AddSourceObject(this);
		// 초기 GE, 레벨, 컨텍스트 핸들을 통해 GE스펙 핸들 생성
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
		// GE스펙 핸들이 유효하다면
		if (EffectSpecHandle.IsValid())
		{
			// 타겟 액터에게 GE 적용
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

/*************************************************************************************************
 * 특정 GC를 실행하여 타겟에게 시각적·청각적 효과 주기
 *
 * @author	조현식
 * @date	2024/12/16
 * @param	
 * @return	
 **************************************************************************************************/
void APanItemBox::InvokeGameplayCue(AActor* Target)
{
	// GC 파라미터
	FGameplayCueParameters Param;
	// 출처 : 아이템 박스
	Param.SourceObject = this;
	// 발동 대상 : 타겟
	Param.Instigator = Target;
	// 위치 : 아이템 박스 위치
	Param.Location = GetActorLocation();
	// 태그를 통해 GC 실행
	ASC->ExecuteGameplayCue(GameplayCueTag, Param);
}
