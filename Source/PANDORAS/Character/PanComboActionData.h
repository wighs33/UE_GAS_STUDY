// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PanComboActionData.generated.h"

/*************************************************************************************************
 * 콤보 공격 관련 데이터
 *
 * @author	조현식
 * @date	2024/11/18
 **************************************************************************************************/
UCLASS()
class PANDORAS_API UPanComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPanComboActionData();

	// 몽타주 이름
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	// 최대 콤보
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	// 프레임 속도
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	// 프레임 별 시간초
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
};
