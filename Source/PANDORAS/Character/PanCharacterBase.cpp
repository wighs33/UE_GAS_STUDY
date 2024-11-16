// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PanCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PanCharacterControlData.h"

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

	// 컨트롤 매니저 그룹에 숄더뷰 컨트롤 데이터 추가
	static ConstructorHelpers::FObjectFinder<UPanCharacterControlData> ShoulderDataRef(TEXT("/Script/PANDORAS.PanCharacterControlData'/Game/Pandoras/Character/Control/DA_Shoulder.DA_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	// 컨트롤 매니저 그룹에 쿼터뷰 컨트롤 데이터 추가
	static ConstructorHelpers::FObjectFinder<UPanCharacterControlData> QuaterDataRef(TEXT("/Script/PANDORAS.PanCharacterControlData'/Game/Pandoras/Character/Control/DA_Quater.DA_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}
}

/*************************************************************************************************
 * 컨트롤 데이터 세터
 *
 * @author	조현식
 * @date	2024/10/14
 * @param	컨트롤 데이터
 * @return	
 **************************************************************************************************/
void APanCharacterBase::SetCharacterControlData(const UPanCharacterControlData* CharacterControlData)
{
	// 폰과 무브먼트 설정 갱신
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

/*************************************************************************************************
 * 콤보 액션 몽타주를 플레이함
 *
 * @author	조현식
 * @date	2024/11/12
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterBase::ProcessComboCommand()
{
	// 스켈레탈 메시로 부터 애님인스턴스 가져옴
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// 애님인스턴스를 통해 [콤보 액션 몽타주]를 플레이
	AnimInstance->Montage_Play(ComboActionMontage);
}

