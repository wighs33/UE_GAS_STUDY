 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\UI\PanUserWidget.h
 *
 * #include "UI/PanUserWidget.h"
 * 기본 UI
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"
#include "PanUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UPanUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner);
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;
	
	
	
	
//public:
//	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwningActor = NewOwner; }
//
//protected:
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
//	TObjectPtr<AActor> OwningActor;
};
