
#pragma once

#include "GameplayTagContainer.h"

#define FDTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define FDTAG_DATA_GOLD FGameplayTag::RequestGameplayTag(FName("Data.Gold"))

#define FDTAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define FDTAG_CHARACTER_ISSHIELD FGameplayTag::RequestGameplayTag(FName("Character.State.IsShield"))

#define FDTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))
