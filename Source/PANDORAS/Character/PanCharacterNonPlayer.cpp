// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PanCharacterNonPlayer.h"

APanCharacterNonPlayer::APanCharacterNonPlayer()
{
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