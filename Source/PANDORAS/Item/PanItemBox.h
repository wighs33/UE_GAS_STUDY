 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Item\PanItemBox.h
 * 
 * #include "Item/PanItemBox.h"
 * 
 * 아이템 박스
 * 
 * GameplayCueTag : GameplayCue.Chest.Open
 *
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "PanItemBox.generated.h"

UCLASS()
class PANDORAS_API APanItemBox : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	APanItemBox();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

protected:
	virtual void PostInitializeComponents() override;

	void ApplyEffectToTarget(AActor* Target);
	void InvokeGameplayCue(AActor* Target);

protected:
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, Category=Box)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = GAS, Meta=(Categories=GameplayCue))
	FGameplayTag GameplayCueTag;
};
