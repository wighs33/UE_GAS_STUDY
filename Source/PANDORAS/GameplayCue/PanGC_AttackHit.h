 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\GameplayCue\PanGC_AttackHit.h
 * 
 * #include "GameplayCue/PanGC_AttackHit.h"
 * 공격이 들어갔을 때 시각적 이펙트
 * 
 * <BPGC_AttackHit>
 * ParticleSystem : 폭발 이펙트
 * GC태그 : GameplayCue.Character.AttackHit
 *
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "PanGC_AttackHit.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UPanGC_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UPanGC_AttackHit();
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=GameplayCue)
	TObjectPtr<class UParticleSystem> ParticleSystem;
};
