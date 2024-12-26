 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Item\PanWeaponItemData.h
 *
 * #include "Item/PanWeaponItemData.h"
 * 무기 아이템 데이터
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Item/PanItemData.h"
#include "PanWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UPanWeaponItemData : public UPanItemData
{
	GENERATED_BODY()
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("PanItemData", GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
};
