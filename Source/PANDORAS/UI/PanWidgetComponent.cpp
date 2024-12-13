// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PanWidgetComponent.h"
#include "UI/PanUserWidget.h"

/*************************************************************************************************
 * 위젯 초기화
 * 위젯에 액터 설정하기
 *
 * @author	조현식
 * @date	2024/12/04
 **************************************************************************************************/
void UPanWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// 이 컴포넌트의 위젯이 있다면
	UPanUserWidget* PanUserWidget = Cast<UPanUserWidget>(GetWidget());
	if (PanUserWidget)
	{
		// 오너를 통해 ASC 설정
		PanUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}