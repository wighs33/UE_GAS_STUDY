// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PanHpBarWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PanCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Tag/PanGameplayTag.h"

/*************************************************************************************************
 * 오너를 통한 ASC 설정
 * 기본 UI(상위 클래스)와 다르게 체력도 갱신
 *
 * @author	조현식
 * @date	2024/12/04
 * @param	오너
 **************************************************************************************************/
void UPanHpBarWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	// ASC가 존재한다면
	if (ASC)
	{
		// 어트리뷰트의 현재 체력 값 변경 시점에 OnHealthChanged 함수 바인딩
		ASC->GetGameplayAttributeValueChangeDelegate(UPanCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UPanHpBarWidget::OnHealthChanged);
		// 어트리뷰트의 최대 체력 값 변경 시점에 OnMaxHealthChanged 함수 바인딩
		ASC->GetGameplayAttributeValueChangeDelegate(UPanCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UPanHpBarWidget::OnMaxHealthChanged);
		// 캐릭터의 무적 상태 태그가 추가되거나 변경되는 시점에 OnInvinsibleTagChanged 함수 바인딩
		ASC->RegisterGameplayTagEvent(TAG_CHARACTER_INVINSIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UPanHpBarWidget::OnInvinsibleTagChanged);
		// 프로그레스 바 컨트롤의 색을 HealthColor로 설정
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		// 캐릭터의 어트리뷰트 모음을 가져온다
		const UPanCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UPanCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
			// 최대 체력이 0보다 크면
			if (CurrentMaxHealth > 0.0f)
			{
				// HP바 갱신
				UpdateHpBar();
			}
		}
	}
}

/*************************************************************************************************
 * 현재 체력 변경 시점 콜백함수
 *
 * @author	조현식
 * @date	2024/12/04
 * @param	
 * @return	
 **************************************************************************************************/
void UPanHpBarWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	// 현재 체력을 새 값으로 변경
	CurrentHealth = ChangeData.NewValue;
	// HP바 갱신
	UpdateHpBar();
}

/*************************************************************************************************
 * 최대 체력 변경 시점 콜백함수
 *
 * @author	조현식
 * @date	2024/12/04
 * @param	
 * @return	
 **************************************************************************************************/
void UPanHpBarWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	// 최대 체력을 새 값으로 변경
	CurrentMaxHealth = ChangeData.NewValue;
	// HP바 갱신
	UpdateHpBar();
}

/*************************************************************************************************
 * 무적태그 변경 시점 콜백함수
 *
 * @author	조현식
 * @date	2024/12/04
 * @param	태그, 부착된 태그 수
 * @return	
 **************************************************************************************************/
void UPanHpBarWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// 부착된 태그가 있다면
	if (NewCount > 0)
	{
		// 프로그레스 바 컨트롤의 색을 InvinsibleColor로 설정
		PbHpBar->SetFillColorAndOpacity(InvinsibleColor);
		// 프로그레스 바의 비율을 1로 설정
		PbHpBar->SetPercent(1.0f);
	}
	else
	{
		// 프로그레스 바 컨트롤의 색을 HealthColor로 설정
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		// HP바 갱신
		UpdateHpBar();
	}
}

/*************************************************************************************************
 * hp바에 표시된 내용 갱신
 *
 * @author	조현식
 * @date	2024/12/04
 * @param	
 * @return	
 **************************************************************************************************/
void UPanHpBarWidget::UpdateHpBar()
{
	// HP 프로그레스 바 컨트롤이 있다면
	if (PbHpBar)
	{
		// 프로그레스 바의 비율을 [현재 체력 / 최대 체력]으로 설정
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}
	// Hp스탯 텍스트가 있다면
	if (TxtHpStat)
	{
		// 텍스트를 [현재 체력 / 최대 체력]으로 표시
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}
