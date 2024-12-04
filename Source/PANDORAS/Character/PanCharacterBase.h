 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Character\PanCharacterBase.h
 *
 * #include "Character\PanCharacterBase.h"
 * 기본 캐릭터
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PanCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class PANDORAS_API APanCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APanCharacterBase();

	FORCEINLINE virtual class UAnimMontage* GetComboActionMontage() const { return ComboActionMontage; }
	FORCEINLINE class UPanComboActionData* GetComboActionData() const { return ComboActionData; }
	//FORCEINLINE class UAnimMontage* GetDeadMontage() const { return DeadMontage; }

protected:
	virtual void SetCharacterControlData(const class UPanCharacterControlData* CharacterControlData);

	//컨트롤 매니저 맵 추가
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UPanCharacterControlData*> CharacterControlManager;

// 콤보 액션 섹션
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPanComboActionData> ComboActionData;

	// PanGA_Attack에서 구현
	//void ProcessComboCommand();
	//void ComboActionBegin();
	//void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	//void SetComboCheckTimer();
	//void ComboCheck();
	//int32 CurrentCombo = 0;
	//FTimerHandle ComboTimerHandle;
	//bool HasNextComboCommand = false;

//// 피격
//protected:
//	virtual void AttackHitCheck() override;
//	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// 사망
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;
};
