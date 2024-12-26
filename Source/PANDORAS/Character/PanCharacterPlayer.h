 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Character\PanCharacterPlayer.h
 *
 * #include "Character/PanCharacterPlayer.h"
 * 확장 캐릭터 (플레이어)
 * 
 * <BP_PanCharacterPlayer>
 * StartAbilities : BPGA_AttackHitCheck, BPGA_SkillHitCheck
 * StartInputAbilities : BPGA_Jump, BPGA_Attack
 * SkillAbilityClass : BPGA_Skill
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Character/PanCharacterBase.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "Interface/PanCharacterItemInterface.h"
#include "PanCharacterPlayer.generated.h"

UENUM()
enum class ECharacterInputType : uint8
{
	Jump = 0,
	Attack,
	Skill
};

//// 아이템 사용을 위한 델리게이트 : 바인딩 하는 함수는 인자가 UPanItemData* 타입 하나여야 한다.
//DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UPanItemData* /*InItemData*/);
//
//// 배열에 넣기 위해 FTakeItemDelegateWrapper를 만들어서 사용
//USTRUCT(BlueprintType)
//struct FTakeItemDelegateWrapper
//{
//	GENERATED_BODY()
//
//	FTakeItemDelegateWrapper() {}
//	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
//	FOnTakeItemDelegate ItemDelegate;
//};

UCLASS()
class PANDORAS_API APanCharacterPlayer : 
	public APanCharacterBase, public IAbilitySystemInterface, public IPanCharacterItemInterface
{
	GENERATED_BODY()

public:
	APanCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE virtual class UAnimMontage* GetSkillActionMontage() const { return SkillActionMontage; }

// 컨트롤
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UPanCharacterControlData* CharacterControlData) override;
	
// 카메라
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// 입력 액션
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void QuaterMove(const FInputActionValue& Value);

	void SetupGASInputComponent();
	void InputPressed(int32 InputId);
	void InputReleased(int32 InputId);

	// 현재 시점
	ECharacterControlType CurrentCharacterControlType = ECharacterControlType::Quater;

// 어빌리티
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC = nullptr;

	// 에디터에서 [기본 어빌리티] 그룹 설정
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;
	
	// 에디터에서 [기본 입력 어빌리티] 그룹 설정
	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<ECharacterInputType, TSubclassOf<class UGameplayAbility>> StartInputAbilities;


// UI
protected:
	UFUNCTION()
	virtual void OnOutOfHealth();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPanWidgetComponent> HpBar;

// 아이템
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UPanItemData> RecentItemData;

	//UPROPERTY()
	//TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void SetItemData(class UPanItemData* InItemData) override { RecentItemData = InItemData; }
	virtual void DrinkPotion(const FGameplayEventData* EventData);
	virtual void ReadScroll(const FGameplayEventData* EventData);

	void EquipWeapon(const FGameplayEventData* EventData);
	void UnequipWeapon(const FGameplayEventData* EventData);

	UPROPERTY(EditAnywhere, Category = Weapon)
	float WeaponRange = 75.f;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float WeaponAttackRate = 100.f;

// 스킬
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayAbility> SkillAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	TObjectPtr<class UAnimMontage> SkillActionMontage;
};
