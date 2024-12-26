 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Interface\PanCharacterItemInterface.h
 *
 * #include "Interface/PanCharacterItemInterface.h"
 * 캐릭터의 아이템 인터페이스
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PanCharacterItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPanCharacterItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PANDORAS_API IPanCharacterItemInterface
{
	GENERATED_BODY()

public:
	virtual void SetItemData(class UPanItemData* InItemData) = 0;
};
