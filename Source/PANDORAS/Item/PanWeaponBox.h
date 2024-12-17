// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PanItemBox.h"
#include "PanWeaponBox.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API APanWeaponBox : public APanItemBox
{
	GENERATED_BODY()
	
protected:
	virtual void NotifyActorBeginOverlap(AActor* Other) override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS, Meta=(Categories=Event))
	FGameplayTag WeaponEventTag;
};
