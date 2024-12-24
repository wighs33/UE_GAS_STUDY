 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Character\PanCharacterNonPlayer.h
 *
 * #include "Character/PanCharacterNonPlayer.h"
 * 확장 캐릭터 (AI적군)
 * 
 * 아래 블루프린트에 의해 게임 시작 후 3초 지나면 무적
 * <BP_PanCharacterNonPlayer>
 * 0. 비긴플레이
 * 1. Give Ability (BPGA_Invinsible)
 * 2. Delay 3초
 * 3. Try Activate Ability By Class (BPGA_Invinsible)
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Character/PanCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "PanCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API APanCharacterNonPlayer : public APanCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	APanCharacterNonPlayer();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetDead() override;

	UFUNCTION()
	virtual void OnOutOfHealth();

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// 데이터 모음
	UPROPERTY()
	TObjectPtr<class UPanCharacterAttributeSet> AttributeSet;

	// 시작 GE
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

	// 레벨
	UPROPERTY(EditAnywhere, Category = GAS)
	float Level;

// UI
protected:
	// HP바
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPanWidgetComponent> HpBar;
};
