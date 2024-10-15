// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PanPlayerState.h"
#include "AbilitySystemComponent.h"

APanPlayerState::APanPlayerState()
{
	// 컴포넌트 생성
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	// 네트워크 동기화
	//ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* APanPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
