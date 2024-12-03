// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PanCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/PanCharacterAttributeSet.h"

APanCharacterNonPlayer::APanCharacterNonPlayer()
{
	// 컴포넌트 생성
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UPanCharacterAttributeSet>(TEXT("AttributeSet"));
}

/*************************************************************************************************
 * ASC 게터
 *
 * @author	조현식
 * @date	2024/12/02
 * @param
 * @return
 **************************************************************************************************/
UAbilitySystemComponent* APanCharacterNonPlayer::GetAbilitySystemComponent() const
{
    return ASC;
}

/*************************************************************************************************
 * 빙의될 때
 *
 * @author	조현식
 * @date	2024/12/02
 * @param	
 * @return	
 **************************************************************************************************/
void APanCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 어빌리티 등록
	ASC->InitAbilityActorInfo(this, this);
}

///*************************************************************************************************
//* 죽었을 때 일정시간초 지나면 사라지도록 구현
//**************************************************************************************************/
//void APanCharacterNonPlayer::SetDead()
//{
//	Super::SetDead();
//
//	FTimerHandle DeadTimerHandle;
//	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
//		[&]()
//		{
//			Destroy();
//		}
//	), DeadEventDelayTime, false);
//}