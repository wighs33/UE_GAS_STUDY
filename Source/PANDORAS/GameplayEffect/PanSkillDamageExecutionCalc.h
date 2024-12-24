 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\GameplayEffect\PanSkillDamageExecutionCalc.h
 *
 * #include "GameplayEffect/PanSkillDamageExecutionCalc.h"
 * 거리에 따른 데미지 계산을 위한 클래스
 * BPGE_SkillDamage에서 사용
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PanSkillDamageExecutionCalc.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UPanSkillDamageExecutionCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
