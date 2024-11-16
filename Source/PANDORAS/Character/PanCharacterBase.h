// Fill out your copyright notice in the Description page of Project Settings.

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

protected:
	virtual void SetCharacterControlData(const class UPanCharacterControlData* CharacterControlData);

	//컨트롤 매니저 맵 추가
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UPanCharacterControlData*> CharacterControlManager;

// 콤보 액션 섹션
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	void ProcessComboCommand();
};
