// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PanCharacterBase.h"

// Sets default values
APanCharacterBase::APanCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APanCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APanCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APanCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

