 /**************************************************************************************************
 * @file	C:\Users\whgus\Desktop\GameProject\PANDORAS\Source\PANDORAS\Tag\PanGamePlayTag.h
 *
 * #include "Tag/PanGamePlayTag.h"
 * 
 * 태그는 이벤트의 이름으로 생각하자
 * 
 * @author	조현식
 * @date	2024-04-06
 *
 * Copyright (c) 2024 LaughLife. All rights reserved
 **************************************************************************************************/

#pragma once

#include "GameplayTagContainer.h"

#define TAG_ACTOR_ROTATE FGameplayTag :: RequestGameplayTag(FName("Actor.Action.Rotate"))
#define TAG_ACTOR_ISROTATING FGameplayTag :: RequestGameplayTag(FName("Actor.State.IsRotating"))
#define TAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define TAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define TAG_CHARACTER_INVINSIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invinsible"))
#define TAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))
#define TAG_EVENT_CHARACTER_WEAPONEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.Equip"))
#define TAG_EVENT_CHARACTER_WEAPONUNEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Weapon.Unequip"))