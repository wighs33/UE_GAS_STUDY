// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PanFountain.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "Tag/PanGameplayTag.h"

// Sets default values
APanFountain::APanFountain()
{
	// 컴포넌트 생성
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	// 종속 관계 설정
	RootComponent = Body;
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	
	// 상대 좌표 설정
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 160.0f));

	// 몸체
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Pandoras/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));
	if (BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}

	// 물
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Pandoras/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));
	if (WaterMeshRef.Object)
	{
		Water->SetStaticMesh(WaterMeshRef.Object);
	}

	// 조명
	Light->SetLightColor(FLinearColor::Blue);
}

UAbilitySystemComponent* APanFountain::GetAbilitySystemComponent() const
{
	return ASC;
}

void APanFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 자동 회전 X
	RotatingMovement->bAutoActivate = false;
	// 회전 비활성화
	RotatingMovement->Deactivate();
	// 어빌리티 시스템 컴포넌트 초기화
	ASC->InitAbilityActorInfo(/*오너액터*/this, /*아바타액터(비주얼만 수행)*/this);

	for (const auto& StartAbility : StartAbilities)
	{
		// 스타트스펙 초기화
		FGameplayAbilitySpec StartSpec(StartAbility);
		// 스펙 : 게임 어빌리티의 정보를 담은 구조체
		
		// 어빌리티 등록
		ASC->GiveAbility(StartSpec);
	}
}

void APanFountain::BeginPlay()
{
	Super::BeginPlay();

	// 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &APanFountain::TimerAction, ActionPeriod, true, 0.0f);
}

/*************************************************************************************************
 * 타이머 주기마다 호출
 * 회전과 멈춤 반복
 *
 * @author	조현식
 * @date	2024/10/12
 **************************************************************************************************/
void APanFountain::TimerAction()
{
	// 태그 초기화
	FGameplayTagContainer TargetTag(TAG_ACTOR_ROTATE);

	// 상태 태그 가져오기
	if (!ASC->HasMatchingGameplayTag(TAG_ACTOR_ISROTATING))
	{
		// 어빌리티 발동
		ASC->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		// 발동 취소
		ASC->CancelAbilities(&TargetTag);
	}
}

