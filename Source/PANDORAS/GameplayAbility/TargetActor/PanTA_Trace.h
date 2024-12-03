 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\GameplayAbility\TargetActor\PanTA_Trace.h
 *
 * #include "GameplayAbility/TargetActor/PanTA_Trace.h"
 * 트레이스를 통해 즉각적으로 검출되는 타겟
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "PanTA_Trace.generated.h"

UCLASS()
class PANDORAS_API APanTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	APanTA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;
	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
};
