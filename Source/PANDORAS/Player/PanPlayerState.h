// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "PanPlayerState.generated.h"

/*************************************************************************************************
 * 플레이어 상태
 *
 * @author	조현식
 * @date	2024/10/14
 **************************************************************************************************/
UCLASS()
class PANDORAS_API APanPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	APanPlayerState();
	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UPanCharacterAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<class UPanCharacterSkillAttributeSet> SkillAttributeSet;
};
