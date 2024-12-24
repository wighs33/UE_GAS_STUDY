 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\GameplayAbility\AbilityTask\PanAT_Trace.h
 *
 * #include "GameplayAbility/AbilityTask/PanAT_Trace.h"
 * 충돌 트레이스 진행 알림
 * PanTA_Trace로 타겟 판정
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PanAT_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	/*델리게이트명*/FTraceResultDelegate, 
	/*인자 타입*/const FGameplayAbilityTargetDataHandle&, 
	/*인자 이름*/TargetDataHandle);

UCLASS()
class PANDORAS_API UPanAT_Trace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UPanAT_Trace();
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", 
	HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UPanAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class APanTA_Trace> TargetActorClass);
	
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class APanTA_Trace> TargetActorClass;
	UPROPERTY()
	TObjectPtr<class APanTA_Trace> SpawnedTargetActor;
};
