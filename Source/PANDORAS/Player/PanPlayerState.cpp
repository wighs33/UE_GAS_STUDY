// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PanPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PanCharacterAttributeSet.h"

APanPlayerState::APanPlayerState()
{
	// 컴포넌트 생성
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UPanCharacterAttributeSet>(TEXT("AttributeSet"));

	// 네트워크 동기화
	//ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* APanPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
