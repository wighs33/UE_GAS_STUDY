// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "PanAT_JumpAndWaitForLanding.generated.h"

// 점프 어빌리티에게 알려줌
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForLandingDelegate);

/*************************************************************************************************
 * 캐릭터 점프 수행 & 점프 종료 알림
 *
 * @author	조현식
 * @date	2024/10/16
 **************************************************************************************************/
UCLASS()
class PANDORAS_API UPanAT_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPanAT_JumpAndWaitForLanding();

	// 블루프린트에서 사용할 수 있도록 지정자 추가 (AbilityTask_PlayMontageAndWait.cpp 참고)
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding", 
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UPanAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	// 블루프린트에서 사용할 수 있도록 지정자 추가
	UPROPERTY(BlueprintAssignable)
	FJumpAndWaitForLandingDelegate OnComplete;

protected:
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);
};
