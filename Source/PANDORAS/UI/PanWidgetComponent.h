 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\UI\PanWidgetComponent.h
 *
 * #include "UI/PanWidgetComponent.h"
 * À§Á¬ ÄÄÆ÷³ÍÆ®
 * 
 * @author	Á¶Çö½Ä
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PanWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UPanWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;
};
