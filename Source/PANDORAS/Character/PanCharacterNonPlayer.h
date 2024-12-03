 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Character\PanCharacterNonPlayer.h
 *
 * #include "Character/PanCharacterNonPlayer.h"
 * 확장 캐릭터 (AI적군)
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

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UPanCharacterAttributeSet> AttributeSet;
};
