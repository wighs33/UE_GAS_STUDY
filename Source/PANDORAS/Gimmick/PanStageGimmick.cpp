// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/PanStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/PanCollision.h"
#include "Character/PanCharacterNonPlayer.h"
#include "Item/PanItemBox.h"

APanStageGimmick::APanStageGimmick()
{
	// 스테이지 (스태틱 메시 컴포넌트) 생성
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	// 스테이지가 루트 컴포넌트
	RootComponent = Stage;
	// 스테이지 애셋 로드
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Pandoras/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	// 오브젝트가 유효하다면
	if (StageMeshRef.Object)
	{
		// 메시 설정하기
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	// 스테이지 트리거 (박스 컴포넌트) 생성
	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	// 박스 크기 설정
	StageTrigger->SetBoxExtent(FVector(775.f, 775.f, 300.f));
	// 스테이지에 종속
	StageTrigger->SetupAttachment(Stage);
	// 위치 설정
	StageTrigger->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	// 충돌 프로필 설정
	StageTrigger->SetCollisionProfileName(CPROFILE_PAN_TRIGGER);
	// 스테이지 트리거가 액터와 오버랩되는 시점에 OnStageTriggerBeginOverlap함수를 바인딩한다.
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &APanStageGimmick::OnStageTriggerBeginOverlap);

	// 스테이지 애셋에 붙은 문 소켓 이름들 저장
	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
	// 문 애셋 로드
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Pandoras/Environment/Props/SM_GATE.SM_GATE'"));
	// 모든 문 소켓마다
	for (FName GateSocket : GateSockets)
	{
		// 문 (스태틱 메시 컴포넌트) 생성
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
		// 메시 설정하기
		Gate->SetStaticMesh(GateMeshRef.Object);
		// 스태이지의 문 소켓에 부착
		Gate->SetupAttachment(Stage, GateSocket);
		// 위치 설정
		Gate->SetRelativeLocation(FVector(0.f, -80.5f, 0.f));
		// 회전 설정
		Gate->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		// 문 그룹에 문 추가
		Gates.Add(GateSocket, Gate);
		// 소켓이름에 트리거 붙이기
		FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
		// 문 트리거 (박스 컴포넌트) 생성
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
		// 박스 크기 설정
		GateTrigger->SetBoxExtent(FVector(100.f, 100.f, 300.f));
		// 스테이지에 종속
		GateTrigger->SetupAttachment(Stage, GateSocket);
		// 위치 설정
		GateTrigger->SetRelativeLocation(FVector(70.f, 0.f, 250.f));
		// 충돌 프로필 설정
		GateTrigger->SetCollisionProfileName(CPROFILE_PAN_TRIGGER);
		// 문 트리거가 액터와 오버랩되는 시점에 OnGateTriggerBeginOverlap함수를 바인딩한다.
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &APanStageGimmick::OnGateTriggerBeginOverlap);
		// 컴포넌트 태그에 소켓 추가
		GateTrigger->ComponentTags.Add(GateSocket);
		// 문 트리거 그룹에 추가
		GateTriggers.Add(GateTrigger);
	}

	// 스테이지 상태 준비 상태로 초기화
	CurrentState = EStageState::READY;
	// 상태 변화 액션 그룹에 (상태, 매칭된 함수) 추가
	StateChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &APanStageGimmick::SetReady)));
	StateChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &APanStageGimmick::SetFight)));
	StateChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &APanStageGimmick::SetChooseReward)));
	StateChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &APanStageGimmick::SetChooseNext)));
	
	// 상대 클래스는 npc 클래스
	OpponentClass = APanCharacterNonPlayer::StaticClass();
	

	// 보상 상자 클래스는 아이템 박스 클래스
	RewardBoxClass = APanItemBox::StaticClass();
	// 문 소켓마다
	for (FName GateSocket : GateSockets)
	{
		// 문 소켓위치/2 = 박스 위치
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
		// 보상상자 위치 그룹에 (문 소켓, 박스 위치) 추가
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}
}

UAbilitySystemComponent* APanStageGimmick::GetAbilitySystemComponent() const
{
	return ASC;
}

/*************************************************************************************************
 * 에디터에서 속성 변경될 시 호출
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 에디터에서 설정한 현재 상태를 업데이트
	SetState(CurrentState);
}

/*************************************************************************************************
 * 스테이지 트리거가 다른 액터와 비긴오버랩될 때 호출
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 전투 상태로 변경
	SetState(EStageState::FIGHT);
}

/*************************************************************************************************
 * 문 트리거가 다른 액터와 비긴오버랩될 때 호출
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 문 트리거의 태그가 하나인지 검증
	check(OverlappedComponent->ComponentTags.Num() == 1);
	// 문 트리거의 태그
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	// 앞에서 2글자로 자르고 [소켓 이름]으로 저장 (+-X, +-Y)
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	// 소켓 존재하는 지 검사
	check(Stage->DoesSocketExist(SocketName));

	// 소켓 위치
	FVector NewLocation = Stage->GetSocketLocation(SocketName);
	// 오버랩 결과
	TArray<FOverlapResult> OverlapResults;
	// 충돌 쿼리 파라미터
	FCollisionQueryParams CollisionQueryParam(
		/*성능 프로파일링*/SCENE_QUERY_STAT(GateTrigger),
		/*단순히트체크*/false,
		/*자기자신은 무시*/this);
	// 충돌 검출 (처리방법 : Overlap, 대상 : Multi, 처리설정 : ByObjectType)
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		/*오버랩 과정에서 검출한 정보*/OverlapResults,
		/*오버랩 중심 위치*/NewLocation,
		/*오버랩할 모양의 회전*/FQuat::Identity,
		/*오브젝트 타입*/FCollisionObjectQueryParams::InitType::AllStaticObjects,
		/*오버랩할 모양*/FCollisionShape::MakeSphere(775.f),
		/*충돌 검사의 추가 옵션*/CollisionQueryParam
	);

	// 검출된 게 없으면 (이미 왔던 곳은 스테이지 생성되어 있기 때문에 새 스테이지 생성안됨)
	if (!bResult)
	{
		// 새 위치에 스테이지 기믹 생성
		GetWorld()->SpawnActor<APanStageGimmick>(NewLocation, FRotator::ZeroRotator);
	}
}

/*************************************************************************************************
 * 모든 문 열기
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::OpenAllGates()
{
	// 문 마다
	for (auto Gate : Gates)
	{
		// 90도 돌려서 열기
		(Gate.Value)->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
}

/*************************************************************************************************
 * 모든 문 닫기
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::CloseAllGates()
{
	// 문 마다
	for (auto Gate : Gates)
	{
		// 각도 0 초기화
		(Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

/*************************************************************************************************
 * 상태 세팅 후 상태 함수 호출
 * 스위치문으로 구현해도 됨
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::SetState(EStageState InNewState)
{
	// 현재 상태 갱신
	CurrentState = InNewState;
	// 상태 변화 액션 그룹에 [현재 상태]가 있다면
	if (StateChangeActions.Contains(CurrentState))
	{
		// 현재 상태에 매칭된 함수 실행
		StateChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
	}
}

/*************************************************************************************************
 * 준비 상태 
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::SetReady()
{
	// 충돌 프로필 설정
	StageTrigger->SetCollisionProfileName(CPROFILE_PAN_TRIGGER);
	// 문 트리거 마다
	for (auto GateTrigger : GateTriggers)
	{
		// 충돌 없애기
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	// 모든 문 열기
	OpenAllGates();
}

/*************************************************************************************************
 * 전투 상태
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::SetFight()
{
	// 스태이지 트리거의 충돌 없애기
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	// 문 트리거 마다
	for (auto GateTrigger : GateTriggers)
	{
		// 충돌 없애기
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	// 모든 문 닫기
	CloseAllGates();

	// 스폰 시간 후에 적 스폰
	GetWorld()->GetTimerManager().SetTimer(
		/*핸들*/OpponentTimerHandle,
		/*대상*/this,
		/*적용함수*/&APanStageGimmick::OpponentSpawn,
		/*주기*/OpponentSpawnTime,
		/*루프*/false);
}

/*************************************************************************************************
 * 보상 상태에 호출
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::SetChooseReward()
{
	// 스태이지 트리거의 충돌 없애기
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	// 문 트리거 마다
	for (auto GateTrigger : GateTriggers)
	{
		// 충돌 없애기
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	// 모든 문 닫기
	CloseAllGates();
	// 보상 박스 스폰
	SpawnRewardBoxes();
}

/*************************************************************************************************
 * 다음 상태에 호출
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::SetChooseNext()
{
	// 스태이지 트리거의 충돌 없애기
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	// 문 트리거 마다
	for (auto GateTrigger : GateTriggers)
	{
		// 충돌 프로필 설정
		GateTrigger->SetCollisionProfileName(CPROFILE_PAN_TRIGGER);
	}
	// 모든 문 열기
	OpenAllGates();
}

/*************************************************************************************************
 * 적 제거됐을 때 호출
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	// 보상 상태로 변경
	SetState(EStageState::REWARD);
}

/*************************************************************************************************
 * 적 스폰
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::OpponentSpawn()
{
	// 적당한 위치를 스폰 위치 잡기
	const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.f;
	// 스폰 위치에 적 스폰
	AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);
	// 적 액터를 npc로 변환성공시
	APanCharacterNonPlayer* PanOpponentCharacter = Cast<APanCharacterNonPlayer>(OpponentActor);
	if (PanOpponentCharacter)
	{
		// 적이 파괴되는 시점에 OnOpponentDestroyed 함수 바인딩
		PanOpponentCharacter->OnDestroyed.AddDynamic(this, &APanStageGimmick::OnOpponentDestroyed);
	}
}

/*************************************************************************************************
 * 보상 트리거에 오버랩되었을 때 호출
 * 다음 상태가 됨
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 보상 박스마다
	for (const auto& RewardBox : RewardBoxes)
	{
		// 약참조이기 때문에 유효성 검사
		if (RewardBox.IsValid())
		{
			// 유효한 아이템 박스 얻기
			APanItemBox* ValidItemBox = RewardBox.Get();
			// 오버랩된 박스가 아니라면 (오버랩된 박스는 박스 자체 로직에 맡기기)
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			if (OverlappedBox != ValidItemBox)
			{
				// 박스 파괴
				ValidItemBox->Destroy();
			}
		}
	}

	// 다음 상태로 변경
	SetState(EStageState::NEXT);
}

/*************************************************************************************************
 * 보상 상자 스폰
 * 보상 상태일 때 호출됨
 *
 * @author	조현식
 * @date	2024/12/25
 * @param	
 * @return	
 **************************************************************************************************/
void APanStageGimmick::SpawnRewardBoxes()
{
	// 보상 상자 위치마다
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		// 적당한 위치에 [월드 스폰 위치]로 설정
		FVector WorldSpawnLocation = GetActorLocation() + RewardBoxLocation.Value + FVector(0.f, 0.f, 30.f);
		// [월드 스폰 위치]에 보상 박스 소환
		AActor* ItemActor = GetWorld()->SpawnActor(RewardBoxClass, &WorldSpawnLocation, &FRotator::ZeroRotator);
		// 아이템 상자로 변환 성공 시
		APanItemBox* RewardBoxActor = Cast<APanItemBox>(ItemActor);
		if (RewardBoxActor)
		{
			// 태그에 문 소켓 추가
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);

			// 보상 트리거 오버랩 시점에 OnRewardTriggerBeginOverlap 함수 바인딩
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &APanStageGimmick::OnRewardTriggerBeginOverlap);
			// 보상 박스 그룹에 추가
			RewardBoxes.Add(RewardBoxActor);
		}
	}
}