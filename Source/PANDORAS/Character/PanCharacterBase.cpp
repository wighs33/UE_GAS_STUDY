// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PanCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APanCharacterBase::APanCharacterBase()
{
	// 컨트롤러의 회전값 사용 여부
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캡슐 크기
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	// 캡슐 충돌 프로필
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// 캐릭터가 움직이는 방향에 맞춰 자동으로 회전하는 지
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 회전 속도
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	// 점프 속력
	GetCharacterMovement()->JumpZVelocity = 700.f;
	// 캐릭터가 공중에 있을 때 플레이어가 얼마나 제어할 수 있는지
	GetCharacterMovement()->AirControl = 0.35f;
	// 걷기 최대 속도
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	// 게임패드를 사용할 때 캐릭터가 걷는 최소 속도
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	// 걷거나 달리는 상태에서 얼마나 빠르게 감속해서 정지하는지
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// 메시 상대 위치, 상대 회전
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	// 메시 에니메이션 모드
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// 메시 충돌 프로필
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	// 캐릭터 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Pandoras/Character/res/SK_CharM_solid.SK_CharM_solid'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	// 애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Pandoras/Animation/ABP_PanCharacter.ABP_PanCharacter_C'"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}

// Called when the game starts or when spawned
void APanCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void APanCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

