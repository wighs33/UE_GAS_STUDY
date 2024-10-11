// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PanFountain.generated.h"

UCLASS()
class PANDORAS_API APanFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	APanFountain();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void TimerAction();

// 외관 컴포넌트
protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Water;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UPointLightComponent> Light;

// 회전
protected:
	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<class URotatingMovementComponent> RotatingMovement;

	UPROPERTY(EditAnywhere, Category = Timer)
	float ActionPeriod = 3.0f;

	FTimerHandle ActionTimer;

};
