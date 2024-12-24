 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\GameplayAbility\TargetActor\PanTA_SphereMultiTrace.h
 *
 * #include "GameplayAbility/TargetActor/PanTA_SphereMultiTrace.h"
 * 구체영역으로 감지되는 다수의 액터
 * BPGA_SkillHitCheck에서 사용
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/TargetActor/PanTA_Trace.h"
#include "PanTA_SphereMultiTrace.generated.h"

UCLASS()
class PANDORAS_API APanTA_SphereMultiTrace : public APanTA_Trace
{
	GENERATED_BODY()
	
protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
