 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Attribute\PanCharacterSkillAttributeSet.h
 *
 * #include "Attribute/PanCharacterSkillAttributeSet.h"
 * 게임 내 플레이어나 적을 구성하는 스킬 데이터 모음
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
#include "PanCharacterSkillAttributeSet.generated.h"

 // 멤버변수당 4개의 편의매크로 제공 (속성에 대한 FGameplayAttribute 객체 반환, 게터, 세터, 초기값)
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class PANDORAS_API UPanCharacterSkillAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	public:
	UPanCharacterSkillAttributeSet();
	ATTRIBUTE_ACCESSORS(UPanCharacterSkillAttributeSet, SkillRange);
	ATTRIBUTE_ACCESSORS(UPanCharacterSkillAttributeSet, MaxSkillRange);
	ATTRIBUTE_ACCESSORS(UPanCharacterSkillAttributeSet, SkillAttackRate);
	ATTRIBUTE_ACCESSORS(UPanCharacterSkillAttributeSet, MaxSkillAttackRate);
	ATTRIBUTE_ACCESSORS(UPanCharacterSkillAttributeSet, SkillEnergy);
	ATTRIBUTE_ACCESSORS(UPanCharacterSkillAttributeSet, MaxSkillEnergy);
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillRange = 800.f;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRange = 1200.f;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillAttackRate = 150.f;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillAttackRate = 300.f;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillEnergy = 100.f;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillEnergy = 100.f;
};
