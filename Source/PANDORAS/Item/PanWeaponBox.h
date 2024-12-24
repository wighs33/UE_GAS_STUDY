 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Item\PanWeaponBox.h
 *
 * #include "Item/PanWeaponBox.h"
 * 무기 상자
 * 
 * WeaponEventTag : Event.Character.Weapon.Equip / Event.Character.Weapon.Unequip
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

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
