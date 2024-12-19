// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PanCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PanCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "PanComboActionData.h"
#include "Physics/PanCollision.h"
#include "Engine/DamageEvents.h"

#include "PANDORAS.h"

// Sets default values
APanCharacterBase::APanCharacterBase()
{
	// 컨트롤러의 회전값 사용 여부
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캡슐 크기
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	// 캡슐의 [충돌 프로필]을 [커스텀 캡슐 프로필]로 설정
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PAN_CAPSULE);

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
	// 메시 충돌 영역 없애기
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

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

	// 콤보 액션 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Pandoras/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	// 콤보 액션 데이터
	static ConstructorHelpers::FObjectFinder<UPanComboActionData> ComboActionDataRef(TEXT("/Script/PANDORAS.PanComboActionData'/Game/Pandoras/Character/ComboAction/DA_ComboAttack.DA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	// 사망 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Pandoras/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	//// 함수가 바인딩된 아이템 착용 시점을 배열에 추가
	//TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APanCharacterBase::EquipWeapon)));
	//TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APanCharacterBase::DrinkPotion)));
	//TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &APanCharacterBase::ReadScroll)));

	// 컴포넌트 생성
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	// 핸드 소켓에 부착
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

/*************************************************************************************************
 * 컨트롤 데이터 세터
 *
 * @author	조현식
 * @date	2024/10/14
 * @param	컨트롤 데이터
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
 * 사망 처리
 *
 * @author	조현식
 * @date	2024/12/05
 **************************************************************************************************/
void APanCharacterBase::SetDead()
{
	// 이동 불가 설정
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	// 사망 몽타주 재생
	PlayDeadAnimation();

	// 충돌감지 끄기
	SetActorEnableCollision(false);
}

/*************************************************************************************************
 * 사망 몽타주 재생
 *
 * @author	조현식
 * @date	2024/12/05
 **************************************************************************************************/
void APanCharacterBase::PlayDeadAnimation()
{
	// 메시로부터 애님인스턴스 얻기
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// 기존 몽타주 중지
	AnimInstance->StopAllMontages(0.0f);
	// 사망 몽타주 재생
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

///*************************************************************************************************
// * 아이템 사용
// *
// * @author	조현식
// * @date	2024/12/19
// * @param	아이템 데이터
// **************************************************************************************************/
//void APanCharacterBase::TakeItem(UPanItemData* InItemData)
//{
//	if (InItemData)
//	{
//		// 먹은 아이템의 타입에 따라 액션이 바인딩되어 있다면 함수 실행
//		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
//	}
//}

///*********************************************************************************************
// * 포션 마시기
// *
// * @author	조현식
// * @date	2024/12/19
// * @param	아이템 데이터
// *********************************************************************************************/
//void APanCharacterBase::DrinkPotion(UPanItemData* InItemData)
//{
//	PAN_LOG(LogALL, Log, TEXT("Drink Potion"));
//}
//
///*********************************************************************************************
// * 무기 장착
// *
// * @author	조현식
// * @date	2024/12/19
// * @param	아이템 데이터
// *********************************************************************************************/
//void APanCharacterBase::EquipWeapon(UPanItemData* InItemData)
//{
//	UPanWeaponItemData* WeaponItemData = Cast<UPanWeaponItemData>(InItemData);
//	if (WeaponItemData)
//	{
//		// 소프트 레퍼러싱 : 로딩이 안되어 있는 상황일 때 로딩시키기
//		if (WeaponItemData->WeaponMesh.IsPending())
//		{
//			WeaponItemData->WeaponMesh.LoadSynchronous();
//		}
//		// 소프트 레퍼런싱된 건 Get 함수로 가져와야됨
//		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
//	}
//}
//
///*********************************************************************************************
// * 주문서 읽기
// *
// * @author	조현식
// * @date	2024/12/19
// * @param	아이템 데이터
// *********************************************************************************************/
//void APanCharacterBase::ReadScroll(UPanItemData* InItemData)
//{
//	PAN_LOG(LogALL, Log, TEXT("Read Scroll"));
//}



///*************************************************************************************************
// * 콤보 액션 몽타주를 플레이함
// **************************************************************************************************/
//void APanCharacterBase::ProcessComboCommand()
//{
//	// 현재 콤보 횟수가 0인지 체크
//	if (CurrentCombo == 0)
//	{
//		// 콤보 액션 시작
//		ComboActionBegin();
//		return;
//	}
//
//	// 콤보 타이머 핸들이 유효한지 체크
//	if (!ComboTimerHandle.IsValid())
//	{
//		// 다음 콤보는 없다고 지정
//		HasNextComboCommand = false;
//	}
//	else
//	{
//		// 다음 콤보가 있다고 지정
//		HasNextComboCommand = true;
//	}
//}
//
///*************************************************************************************************
// * 콤보 액션 시작
// **************************************************************************************************/
//void APanCharacterBase::ComboActionBegin()
//{
//	// 콤보 횟수 1 초기화
//	CurrentCombo = 1;
//	// 캐릭터 이동 없애기
//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
//
//	// 공격 속도
//	const float AttackSpeedRate = 1.0f;
//	// 메시로부터 애님 인스턴스 얻기
//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	// 지정한 속도로 콤보 공격 몽타주 재생
//	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);
//	
//	// 종료 델리게이트
//	FOnMontageEnded EndDelegate;
//	// 종료 델리게이트와 ComboActionEnd함수 바인딩하기
//	EndDelegate.BindUObject(this, &APanCharacterBase::ComboActionEnd);
//	// 몽타주가 끝나면 콤보 종료 함수 호출 예약
//	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);
//
//	// 콤보 타이머 핸들 초기화
//	ComboTimerHandle.Invalidate();
//	// 콤보 체크 타이머
//	SetComboCheckTimer();
//}
//
///*************************************************************************************************
// * 콤보 종료
// * 몽타주 끝날때 호출됨
// **************************************************************************************************/
//void APanCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
//{
//	// 콤보 전 상태로 초기화
//	ensure(CurrentCombo != 0);
//	// 콤보 횟수 0 초기화
//	CurrentCombo = 0;
//	// 캐릭터 움직임을 걷기 상태로 변경
//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
//}

///*************************************************************************************************
// * 콤보 체크할 타이머 설정
// **************************************************************************************************/
//void APanCharacterBase::SetComboCheckTimer()
//{
//	// 콤보 횟수 - 1 = 콤보 인덱스
//	int32 ComboIndex = CurrentCombo - 1;
//	// EffectiveFrameCount배열에서 ComboIndex가 유효한 인덱스인지 확인
//	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));
//
//	// 공격 스피드 속도
//	const float AttackSpeedRate = 1.0f;
//	// (콤보 프레임 / 프레임 속도) / 공격 스피드 속도 = 콤보 시간
//	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
//	// 콤보 시간이 0보다 큰 지 체크
//	if (ComboEffectiveTime > 0.0f)
//	{
//		// 콤보 시간동안 콤보 진행되었는 지 확인
//		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &APanCharacterBase::ComboCheck, ComboEffectiveTime, false);
//	}
//}
//
///*************************************************************************************************
// * 콤보 체크
// * 데이터 애셋에서 가져온 시간에 따라 다음 몽타주 섹션으로 건너뜀
// **************************************************************************************************/
//void APanCharacterBase::ComboCheck()
//{
//	// 콤보 타이머 핸들 초기화
//	ComboTimerHandle.Invalidate();
//	// 다음 콤보 커맨드 여부 체크
//	if (HasNextComboCommand)
//	{
//		// 메시로 부터 애님 인스턴스 얻기
//		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//
//		// [현재 콤보 횟수 + 1]이 [최대 콤보 횟수] 내에 있는 지 검사
//		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
//
//		// [몽타주이름][콤보횟수]
//		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
//		// 앞서만든 이름으로 찾아서 다음 몽타주 섹션으로 건너뛰기
//		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
//
//		// 콤보 체크 타이머 재시작
//		SetComboCheckTimer();
//		// 다음 콤보는 없다고 지정
//		HasNextComboCommand = false;
//	}
//}

// /*************************************************************************************************
// * 공격 판정
// * 공격 인터페이스의 가상 함수
// **************************************************************************************************/
//void APanCharacterBase::AttackHitCheck()
//{
//	// 공격 판정을 위한 변수
//	FHitResult OutHitResult;
//	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack)/*분석*/, false/*복잡한 폴리전 사용여부*/, this);
//	const float AttackRange = 40.0f;
//	const float AttackRadius = 50.0f;
//	const float AttackDamage = 30.0f;
//	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
//	const FVector End = Start + GetActorForwardVector() * AttackRange;
//
//	// 충돌 검출
//	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult/*충돌 결과 저장됨*/, Start/*검사 시작 위치*/, End/*검사 종료 위치*/, FQuat::Identity,
//		CCHANNEL_PDACTION/*트레이스 채널*/, FCollisionShape::MakeSphere(AttackRadius)/*검사할 영역*/, Params/*정확도*/);
//	if (HitDetected)
//	{
//		FDamageEvent DamageEvent;
//		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
//	}
//
//	// 캡슐 그리기
//#if ENABLE_DRAW_DEBUG
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsuleHalfHeight = AttackRange * 0.5f;
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin/*원점 위치*/, CapsuleHalfHeight/*원점-반구원점 길이*/, AttackRadius/*양끝 반구 반지름*/,
//		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f/*지속시간초*/);
//#endif
//
//}
//
///*************************************************************************************************
//* 데미지를 전달받음
//**************************************************************************************************/
//float APanCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//
//	// 임시로 바로 사망처리
//	SetDead();
//
//	return DamageAmount;
//}
//