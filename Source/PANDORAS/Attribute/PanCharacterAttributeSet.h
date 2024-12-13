 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Attribute\PanCharacterAttributeSet.h
 *
 * #include "CoreMinimal.h"
 * 게임 내 플레이어나 적을 구성하는 데이터 모음
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PanCharacterAttributeSet.generated.h"

// 멤버변수당 4개의 편의매크로 제공 (속성에 대한 FGameplayAttribute 객체 반환, 게터, 세터, 초기값)
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// 1대다 블루프린트 호환 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);

UCLASS()
class PANDORAS_API UPanCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPanCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UPanCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UPanCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UPanCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UPanCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UPanCharacterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UPanCharacterAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UPanCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UPanCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UPanCharacterAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	mutable FOutOfHealthDelegate OnOutOfHealth;

protected:
	// 현재 공격 범위
	UPROPERTY(BlueprintReadOnly, Category="Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange = 100.f;

	// 최대 공격 범위
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange = 300.f;

	// 현재 공격 반지름
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius = 50.f;

	// 최대 공격 반지름
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius = 150.f;

	// 현재 공격력
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate = 30.f;

	// 최대 공격력
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate = 100.f;

	// 현재 체력
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// 최대 체력
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth = 100.f;

	// 데미지
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage = 0.f;

	// 체력 고갈 플래그
	bool bOutOfHealth = false;

	// 체력 이펙트에서 접근할 수 있도록 friend 지정
	friend class UPanGE_AttackDamage;
};
