 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Gimmick\PanStageGimmick.h
 *
 * #include "Gimmick/PanStageGimmick.h"
 * 스테이지 기믹
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "PanStageGimmick.generated.h"

DECLARE_DELEGATE(FOnStageChangedDelegate);
USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()
	FStageChangedDelegateWrapper() {}
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageDelegate(InDelegate) {}
	FOnStageChangedDelegate StageDelegate;
};
UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,
	NEXT
};

UCLASS()
class PANDORAS_API APanStageGimmick : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	APanStageGimmick();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

// 스테이지
protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Stage;
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;
	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
// 문
protected:
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;
	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OpenAllGates();
	void CloseAllGates();

// 스테이지 상태
protected:
	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;
	void SetState(EStageState InNewState);
	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StateChangeActions;
	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

// 전투
protected:
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APanCharacterNonPlayer> OpponentClass;
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime = 2.f;
	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);
	FTimerHandle OpponentTimerHandle;
	void OpponentSpawn();

// 보상
protected:
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APanItemBox> RewardBoxClass;

	// 약참조 : 가비지컬렉션이 오브젝트를 소멸시키는 것을 막지않는다, 파괴되었을 수도 있는 객체를 안전하게 참조한다
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<class APanItemBox>> RewardBoxes;

	TMap<FName, FVector> RewardBoxLocations;
	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SpawnRewardBoxes();
};
