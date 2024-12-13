#pragma once

#include "GameplayTagContainer.h"

#define TAG_ACTOR_ROTATE FGameplayTag :: RequestGameplayTag(FName("Actor.Action.Rotate"))
#define TAG_ACTOR_ISROTATING FGameplayTag :: RequestGameplayTag(FName("Actor.State.IsRotating"))
#define TAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define TAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define TAG_CHARACTER_INVINSIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invinsible"))