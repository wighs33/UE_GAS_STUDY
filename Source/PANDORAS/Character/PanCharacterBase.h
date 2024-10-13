// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PanCharacterBase.generated.h"

UCLASS()
class PANDORAS_API APanCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APanCharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
