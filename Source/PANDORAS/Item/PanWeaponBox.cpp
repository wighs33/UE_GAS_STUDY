// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PanWeaponBox.h"
#include "AbilitySystemBlueprintLibrary.h"

/*************************************************************************************************
 * 다른 액터와 오버랩 발생될 때 호출 (오버라이드)
 *
 * @author	조현식
 * @date	2024/12/17
 * @param	
 * @return	
 **************************************************************************************************/
void APanWeaponBox::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	// 어빌리티의 기본 함수인 ActivateAbility의 TriggerEventData로 데이터가 전송됨
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Other, WeaponEventTag, FGameplayEventData());
}
