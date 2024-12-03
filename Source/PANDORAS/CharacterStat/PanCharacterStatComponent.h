 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\CharacterStat\PanCharacterStatComponent.h
 *
 * #include "CharacterStat/PanCharacterStatComponent.h"
 * 캐릭터 스탯(hp, 데미지) 컴포넌트
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PanCharacterStatComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float/*, CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANDORAS_API UPanCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPanCharacterStatComponent();

//protected:
//	virtual void BeginPlay() override;
//
//public:
//	FOnHpZeroDelegate OnHpZero;
//	FOnHpChangedDelegate OnHpChanged;
//
//	FORCEINLINE float GetMaxHp() { return MaxHp; }
//	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
//	float ApplyDamage(float InDamage);
//
//protected:
//	void SetHp(float NewHp);
//
//	UPROPERTY(VisibleInstanceOnly, Category = Stat)
//	float MaxHp;
//
//	// 매번 새롭게 진행되기 때문에 디스크에 저장할 필요 없음 -> Transient 키워드 지정
//	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
//	float CurrentHp;
};
