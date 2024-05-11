// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDCharacterStatComponent.h"
#include "GameData/FDGameSingleton.h"
// Sets default values
UFDCharacterStatComponent::UFDCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.0f;
}

// Called when the game starts or when spawned
void UFDCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UFDCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UFDGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(UFDGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
}

float UFDCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UFDCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}
