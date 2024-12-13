// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PanUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

/*************************************************************************************************
 * 오너를 통한 ASC 설정
 *
 * @author	조현식
 * @date	2024/12/04
 * @param	오너
 **************************************************************************************************/
void UPanUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	// UI의 오너가 유효하다면
	if (IsValid(InOwner))
	{
		// 액터의 ASC를 가져온다
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

/*************************************************************************************************
 * ASC 게터
 *
 * @author	조현식
 * @date	2024/12/04
 * @return	ASC
 **************************************************************************************************/
UAbilitySystemComponent* UPanUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
