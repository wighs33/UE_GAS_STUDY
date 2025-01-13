// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PanCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PanCharacterControlData.h"
#include "AbilitySystemComponent.h"
#include "Player/PanPlayerState.h"
#include "UI/PanWidgetComponent.h"
#include "UI/PanUserWidget.h"
#include "Attribute/PanCharacterAttributeSet.h"
#include "Tag/PanGamePlayTag.h"
#include "Item/PanWeaponItemData.h"

#include "PANDORAS.h"

#include "GameFramework/CharacterMovementComponent.h"

APanCharacterPlayer::APanCharacterPlayer()
{
	// 리플리케이트 허용
	SetReplicates(true);
	SetReplicateMovement(true);

	// 컴포넌트 생성
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	HpBar = CreateDefaultSubobject<UPanWidgetComponent>(TEXT("Widget"));

	// 종속 관계 설정
	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	HpBar->SetupAttachment(GetMesh());

	// 스프링암 길이
	CameraBoom->TargetArmLength = 400.f;

	// 폰의 회전을 따를 지
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera->bUsePawnControlRotation = false;

	// 입력 액션 애셋 로드
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
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSkillRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Pandoras/Input/Actions/IA_Skill.IA_Skill'"));
	if (nullptr != InputActionSkillRef.Object)
	{
		SkillAction = InputActionSkillRef.Object;
	}

	// HP바 위치는 캐릭터 머리 위
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	// HP바 애셋 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/Pandoras/UI/WBP_HpBar.WBP_HpBar_C"));
	// 클래스가 유효하다면
	if (HpBarWidgetRef.Class)
	{
		// 참조할 위젯
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		// 화면 공간에 렌더링
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		// 크기는 2D로 설정
		HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
		// 충돌 영역 없애기
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 스킬 몽타주 로드
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKillActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Pandoras/Animation/AM_SkillAttack.AM_SkillAttack'"));
	// 오브젝트가 유효하다면
	if (SKillActionMontageRef.Object)
	{
		// TObjectPtr형식으로 저장
		SkillActionMontage = SKillActionMontageRef.Object;
	}

	//// 함수가 바인딩된 아이템 착용 시점을 배열에 추가
	//TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APanCharacterPlayer::EquipWeapon)));
	//TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APanCharacterPlayer::DrinkPotion)));
	//TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APanCharacterPlayer::ReadScroll)));
}

void APanCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 현재 시점의 컨트롤 데이터 적용
	SetCharacterControl(CurrentCharacterControlType);
}

/*************************************************************************************************
 * ASC 게터
 *
 * @author	조현식
 * @date	2024/12/02
 * @param	
 * @return	
 **************************************************************************************************/
UAbilitySystemComponent* APanCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

/*************************************************************************************************
 * 빙의될 때
 *
 * @author	조현식
 * @date	2024/12/02
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 플레이어 스테이트 얻기
	if (APanPlayerState* PanPlayerState = GetPlayerState<APanPlayerState>())
	{
		// 플레이어 스테이트에서 ASC 얻기
		ASC = PanPlayerState->GetAbilitySystemComponent();
		// 어빌리티 등록
		ASC->InitAbilityActorInfo(PanPlayerState, this);

		// 무기 장착 태그에 신호가 오는 시점에 EquipWeapon 함수 바인딩 (SendGameplayEventToActor 함수를 통해 신호 전달받음)
		ASC->GenericGameplayEventCallbacks.FindOrAdd(TAG_EVENT_CHARACTER_WEAPONEQUIP).AddUObject(this, &APanCharacterPlayer::EquipWeapon);
		// 무기 탈착 태그에 신호가 오는 시점에 UnequipWeapon 함수 바인딩
		ASC->GenericGameplayEventCallbacks.FindOrAdd(TAG_EVENT_CHARACTER_WEAPONUNEQUIP).AddUObject(this, &APanCharacterPlayer::UnequipWeapon);
		// 포션 사용 태그에 신호가 오는 시점에 DrinkPotion 함수 바인딩
		ASC->GenericGameplayEventCallbacks.FindOrAdd(TAG_EVENT_CHARACTER_POTIONDRINK).AddUObject(this, &APanCharacterPlayer::DrinkPotion);
		// 주문서 사용 태그에 신호가 오는 시점에 ReadScroll 함수 바인딩
		ASC->GenericGameplayEventCallbacks.FindOrAdd(TAG_EVENT_CHARACTER_SCROLLREAD).AddUObject(this, &APanCharacterPlayer::ReadScroll);

		// 어트리뷰트 모음 얻는데 성공한다면
		const UPanCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UPanCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			// 체력 고갈 시점에 콜백함수 바인딩
			CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
		}

		for (const auto& StartAbility : StartAbilities)
		{
			// 스타트스펙 초기화
			FGameplayAbilitySpec StartSpec(StartAbility);
			// 어빌리티 등록
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			// 스타트스펙 초기화
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			// StartInputAbilities의 키를 스타트스펙의 아이디로 사용
			StartSpec.InputID = static_cast<int32>(StartInputAbility.Key);
			// 어빌리티 등록
			ASC->GiveAbility(StartSpec);
		}

		// 멀티플레이일때 호출
		SetupGASInputComponent();
		// 플레이어 컨트롤러로 캐스팅
		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		// 어빌리티 시스템 디버깅 실행
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

/*************************************************************************************************
 * 입력 값 초기화가 필요할 시(게임 시작, 액터 소유) 호출
 *
 * @author	조현식
 * @date	2024/11/17
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 향상된 입력 컴포넌트로 확장
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	// 입력 액션과 액션 함수 바인딩 (이동과 시야는 GAS로 분리하지 않는 것이 더 좋다고 판단)
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &APanCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &APanCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &APanCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &APanCharacterPlayer::QuaterMove);
	
	// 게임 어빌리티 시스템으로 입력 바인딩
	SetupGASInputComponent();
}
//
//bool APanCharacterPlayer::ServerRPCInputPressed_Validate(int32 InputId)
//{
//	return true;
//}
//void APanCharacterPlayer::ServerRPCInputPressed_Implementation(int32 InputId)
//{
//	MulticastRPCInputPressed(InputId);
//}
//void APanCharacterPlayer::MulticastRPCInputPressed_Implementation(int32 InputId)
//{
//	// ASC에 등록된 스펙을 검사해 입력에 매핑된 GA 찾기
//	if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId))
//	{
//		// 스펙에 입력 알려줌
//		Spec->InputPressed = true;
//		if (Spec->IsActive())
//		{
//			// GA가 발동 중이면 입력이 왔다는 신호 전달
//			ASC->AbilitySpecInputPressed(*Spec);
//		}
//		else
//		{
//			// GA가 발동 중이 아니면 새롭게 발동
//			ASC->TryActivateAbility(Spec->Handle);
//		}
//	}
//}

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
 * 시점에 따라 다른 컨트롤 데이터 적용 (입력 매핑 컨텍스트 갱신)
 *
 * @author	조현식
 * @date	2024/10/14
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	if (!IsLocallyControlled())
	{
		return;
	}

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

/*************************************************************************************************
 * 숄더 뷰일 때 이동 키 입력 시 호출
 *
 * @author	조현식
 * @date	2024/11/17
 * @param
 * @return
 **************************************************************************************************/

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

/*************************************************************************************************
 * 숄더 뷰일 때 시야 조정 시 호출
 *
 * @author	조현식
 * @date	2024/11/17
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	// 회전 오프셋(Yaw, Roll)
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 컨트롤러만 Z축 중심 오프셋만큼 회전
	AddControllerYawInput(LookAxisVector.X);
	// 컨트롤러만 Y축 중심 오프셋만큼 회전
	AddControllerPitchInput(LookAxisVector.Y);
}

/*************************************************************************************************
 * 쿼터 뷰일 때 이동 키 입력 시 호출
 *
 * @author	조현식
 * @date	2024/11/17
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	// 이동 벡터
	FVector2D MovementVector = Value.Get<FVector2D>();
	// 이동 오프셋
	float MovementVectorSize = 1.f;
	// 이동 벡터의 길이 제곱
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.f)
	{
		// 1보다 크면 단위벡터화
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.f;
	}
	else
	{
		// 1보다 작거나 같으면 이동 오프셋은 이동 벡터의 길이
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	// 이동 방향 얻기
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);
	// MoveDirection을 컨트롤러가 조종하는 폰의 X축으로 사용
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	// 이동 방향으로 오프셋만큼 이동
	AddMovementInput(MoveDirection, MovementVectorSize);
}

/*************************************************************************************************
 * 점프와 공격은 [게임 어빌리티 시스템]으로 입력 받음
 *
 * @author	조현식
 * @date	2024/11/17
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::SetupGASInputComponent()
{
	FString str = ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER")));

	// 유효성 검사
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		// 향상된 입력 컴포넌트로 확장
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		// 입력 액션과 액션 함수 바인딩
		EnhancedInputComponent->BindAction(
			JumpAction, 
			ETriggerEvent::Triggered, 
			this, 
			&APanCharacterPlayer::InputPressed, 
			static_cast<int32>(ECharacterInputType::Jump));
		EnhancedInputComponent->BindAction(
			JumpAction, 
			ETriggerEvent::Completed, 
			this, 
			&APanCharacterPlayer::InputReleased, 
			static_cast<int32>(ECharacterInputType::Jump));
		EnhancedInputComponent->BindAction(
			AttackAction, 
			ETriggerEvent::Triggered, 
			this, 
			&APanCharacterPlayer::InputPressed, 
			static_cast<int32>(ECharacterInputType::Attack));

		// 에디터에서 어빌리티를 설정하지 않고 무기 장착 시에 코드로 ASC에 어빌리티를 부여함
		EnhancedInputComponent->BindAction(
			SkillAction, 
			ETriggerEvent::Triggered, 
			this, 
			&APanCharacterPlayer::InputPressed, 
			static_cast<int32>(ECharacterInputType::Skill));
	}
}

/*************************************************************************************************
 * 스펙 ID의 입력이 들어왔을 때
 *
 * @author	조현식
 * @date	2024/11/17
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::InputPressed(int32 InputId)
{
	// ASC에 등록된 스펙을 검사해 입력에 매핑된 GA 찾기
	if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId))
	{
		// 스펙에 입력 알려줌
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			// GA가 발동 중이면 입력이 왔다는 신호 전달
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			// GA가 발동 중이 아니면 새롭게 발동
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

/*************************************************************************************************
 * 스펙 ID의 입력이 해제되었을 때
 *
 * @author	조현식
 * @date	2024/11/17
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::InputReleased(int32 InputId)
{
	// ASC에 등록된 스펙을 검사해 입력에 매핑된 GA 찾기
	if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId))
	{
		// 스펙에 입력종료 알려줌
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			// GA에게 입력이 끝났다는 신호 전달
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

/*************************************************************************************************
 * 체력 고갈 시점에 호출
 *
 * @author	조현식
 * @date	2024/12/05
 * @param
 * @return
 **************************************************************************************************/
void APanCharacterPlayer::OnOutOfHealth()
{
	SetDead();
}

/*************************************************************************************************
 * 무기 장착
 *
 * @author	조현식
 * @date	2024/12/17
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::EquipWeapon(const FGameplayEventData* EventData)
{
	// 무기 설정이 가능하다면
	if (Weapon)
	{
		// 무기 아이템 데이터라면 
		UPanWeaponItemData* WeaponItemData = Cast<UPanWeaponItemData>(RecentItemData);
		if (WeaponItemData)
		{
			// 무기 애셋이 로드되지 않았다면 (소프트 레퍼런싱 : 애셋 데이터가 필요한 시점에 로딩하는 방법)
			if (WeaponItemData->WeaponMesh.IsPending())
			{
				// 동기방식으로 애셋을 로드
				WeaponItemData->WeaponMesh.LoadSynchronous();
			}
			// 무기 메시 설정(소프트 레퍼런싱된 건 Get 함수로 가져와야됨)
			Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
		}

		// GA스펙 생성
		FGameplayAbilitySpec NewSkillSpec(SkillAbilityClass);
		// 인풋 아이디를 2번으로 설정
		NewSkillSpec.InputID = static_cast<int32>(ECharacterInputType::Skill);
		// 해당 클래스가 어빌리티를 부여하지 않았다면
		if (!ASC->FindAbilitySpecFromClass(SkillAbilityClass))
		{
			// 어빌리티 부여
			ASC->GiveAbility(NewSkillSpec);
		}

		// 캐릭터 공격범위 가져오기
		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UPanCharacterAttributeSet::GetAttackRangeAttribute());
		// 캐릭터 공격력 가져오기
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UPanCharacterAttributeSet::GetAttackRateAttribute());
		// 캐릭터 기본 공격 범위 증가 (+무기 공격 범위)
		ASC->SetNumericAttributeBase(UPanCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange + WeaponRange);
		// 캐릭터 기본 공격력 증가 (+무기 공격력)
		ASC->SetNumericAttributeBase(UPanCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate + WeaponAttackRate);
	}
}

/*************************************************************************************************
 * 무기 탈착
 *
 * @author	조현식
 * @date	2024/12/17
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterPlayer::UnequipWeapon(const FGameplayEventData* EventData)
{
	// 무기 설정이 가능하다면
	if (Weapon)
	{
		// 캐릭터 공격범위 가져오기
		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UPanCharacterAttributeSet::GetAttackRangeAttribute());
		// 캐릭터 공격력 가져오기
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UPanCharacterAttributeSet::GetAttackRateAttribute());
		// 캐릭터 기본 공격 범위 감소 (-무기 공격 범위)
		ASC->SetNumericAttributeBase(UPanCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange - WeaponRange);
		// 캐릭터 기본 공격력 감소 (-무기 공격력)
		ASC->SetNumericAttributeBase(UPanCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate - WeaponAttackRate);
		
		// 해당 클래스가 어빌리티를 부여했다면
		FGameplayAbilitySpec* SkillAbilitySpec = ASC->FindAbilitySpecFromClass(SkillAbilityClass);
		if (SkillAbilitySpec)
		{
			// 해당 어빌리티 제거
			ASC->ClearAbility(SkillAbilitySpec->Handle);
		}

		// 메시 없애기
		Weapon->SetSkeletalMesh(nullptr);
	}
}

/*********************************************************************************************
 * 포션 마시기
 *
 * @author	조현식
 * @date	2024/12/19
 * @param	아이템 데이터
 *********************************************************************************************/
void APanCharacterPlayer::DrinkPotion(const FGameplayEventData* EventData)
{
	PAN_LOG(LogALL, Warning, TEXT("Drink Potion"));
}

/*********************************************************************************************
 * 주문서 읽기
 *
 * @author	조현식
 * @date	2024/12/19
 * @param	아이템 데이터
 *********************************************************************************************/
void APanCharacterPlayer::ReadScroll(const FGameplayEventData* EventData)
{
	PAN_LOG(LogALL, Warning, TEXT("Read Scroll"));
}

///*************************************************************************************************
// * 아이템 사용
// *
// * @author	조현식
// * @date	2024/12/19
// * @param	아이템 데이터
// **************************************************************************************************/
//void APanCharacterPlayer::TakeItem(UPanItemData* InItemData)
//{
//	if (InItemData)
//	{
//		// 먹은 아이템의 타입에 따라 매칭되는 함수 실행
//		TakeItemActions[static_cast<uint8>(InItemData->Type)].ItemDelegate.ExecuteIfBound(InItemData);
//	}
//}
//