// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PanCharacterBase.h"
#include "PanCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API APanCharacterPlayer : public APanCharacterBase
{
	GENERATED_BODY()

public:
	APanCharacterPlayer();
	
// 카메라
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
};
