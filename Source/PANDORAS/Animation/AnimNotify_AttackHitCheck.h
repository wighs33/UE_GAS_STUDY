 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Animation\AnimNotify_AttackHitCheck.h
 *
 * #include "Animation/AnimNotify_AttackHitCheck.h"
 * 공격 탐지 노티파이
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_AttackHitCheck.generated.h"

UCLASS()
class PANDORAS_API UAnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_AttackHitCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;

	UPROPERTY(EditAnywhere)
	float ComboAttackLevel = 1.0f;
};
