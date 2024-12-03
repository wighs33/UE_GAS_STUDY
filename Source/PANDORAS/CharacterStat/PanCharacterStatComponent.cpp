// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/PanCharacterStatComponent.h"

UPanCharacterStatComponent::UPanCharacterStatComponent()
{
	//MaxHp = 200.0f;
	//CurrentHp = MaxHp;
}

//void UPanCharacterStatComponent::BeginPlay()
//{
//	Super::BeginPlay();
//
//	SetHp(MaxHp);
//}
//
///*************************************************************************************************
// * 데미지 적용
// *
// * @author	조현식
// * @date	2024/11/30
// * @param	실제데미지
// * @return	적용데미지
// **************************************************************************************************/
//float UPanCharacterStatComponent::ApplyDamage(float InDamage)
//{
//	const float PrevHp = CurrentHp;
//	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
//
//	//hp 저장 - hp 변경 시 알림
//	SetHp(PrevHp - ActualDamage);
//	if (CurrentHp <= KINDA_SMALL_NUMBER)
//	{
//		// hp = 0 알림을 구독한 모든 곳에 알리기
//		OnHpZero.Broadcast();
//	}
//
//	return ActualDamage;
//}
//
///*************************************************************************************************
// * hp 저장
// *
// * @author	조현식
// * @date	2024/11/30
// * @param	새 Hp
// **************************************************************************************************/
//void UPanCharacterStatComponent::SetHp(float NewHp)
//{
//	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
//
//	// hp변경 알림을 구독한 모든 곳에 알리고 현재 hp값 보내기
//	OnHpChanged.Broadcast(CurrentHp);
//}

