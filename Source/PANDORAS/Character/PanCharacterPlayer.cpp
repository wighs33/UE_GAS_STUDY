// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PanCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PanCharacterControlData.h"

APanCharacterPlayer::APanCharacterPlayer()
{
	// 컴포넌트 생성
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	// 종속 관계 설정
	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// 스프링암 길이
	CameraBoom->TargetArmLength = 400.f;

	// 폰의 회전을 따를 지
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera->bUsePawnControlRotation = false;

	// 입력 액션 로드
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pandoras/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pandoras/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pandoras/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pandoras/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pandoras/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pandoras/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}
}

void APanCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 현재 시점의 컨트롤 데이터 적용
	SetCharacterControl(CurrentCharacterControlType);
}

void APanCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 향상된 입력 컴포넌트로 확장
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// 입력 액션과 액션 함수 바인딩
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APanCharacterPlayer::Attack);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &APanCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &APanCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &APanCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &APanCharacterPlayer::QuaterMove);
}

/*************************************************************************************************
 * 숄더뷰와 쿼터뷰 시점 사이의 교체
 *
 * @author	조현식
 * @date	2024/10/14
 * @param
 * @return
 **************************************************************************************************/
void APanCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		// 숄더뷰 컨트롤 데이터 적용
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		// 쿼터뷰 컨트롤 데이터 적용
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

/*************************************************************************************************
 * 시점에 따라 다른 컨트롤 데이터 적용
 *
 * @author	조현식
 * @date	2024/10/14
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	// 해당 시점의 컨트롤 데이터
	UPanCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);
	// 새로운 컨트롤 데이터 적용
	SetCharacterControlData(NewCharacterControl);
	// 현재 컨트롤러 얻기
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	// 서브 시스템 얻기
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		// 서브 시스템의 입력 매핑 컨텍스트 모두 제거
		Subsystem->ClearAllMappings();
		// 새 컨트롤 데이터의 입력 매핑 컨텍스트 얻기
		if (UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext)
		{
			// 새 입력 매핑 컨텍스트 최우선으로 사용
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
	// 현재 시점 갱신
	CurrentCharacterControlType = NewCharacterControlType;
}

/*************************************************************************************************
 * 컨트롤 데이터 세터
 *
 * @author	조현식
 * @date	2024/10/14
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::SetCharacterControlData(const UPanCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	// 스프링암 설정 갱신
	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void APanCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	// 이동 오프셋
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 컨트롤러의 Z축 회전방향으로 초기화
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// YawRotation기반으로 매트릭스를 만들고 X축과 Y축 벡터 얻기
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// X축과 Y축 방향으로 오프셋만큼 이동(에디터에서 스위즐 값에 의해 전방은 X축, 좌우측은 Y축으로 변경됨)
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void APanCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	// 회전 오프셋(Yaw, Roll)
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 컨트롤러만 Z축 중심 오프셋만큼 회전
	AddControllerYawInput(LookAxisVector.X);
	// 컨트롤러만 Y축 중심 오프셋만큼 회전
	AddControllerPitchInput(LookAxisVector.Y);
}

void APanCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	// 이동 벡터
	FVector2D MovementVector = Value.Get<FVector2D>();
	// 이동 오프셋
	float MovementVectorSize = 1.0f;
	// 이동 벡터의 길이 제곱
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		// 1보다 크면 단위벡터화
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		// 1보다 작거나 같으면 이동 오프셋은 이동 벡터의 길이
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	// 이동 방향 얻기
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	// MoveDirection을 컨트롤러가 조종하는 폰의 X축으로 사용
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	// 이동 방향으로 오프셋만큼 이동
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void APanCharacterPlayer::Attack()
{
}
