// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/FDCharacterAttributeSet.h"
#include "FDProjectGAS.h"

UFDCharacterAttributeSet::UFDCharacterAttributeSet() :
	AttackRange(100.0f),
	AttackRadius(50.f),
	AttackRate(30.0f),
	MaxAttackRange(300.0f),
	MaxAttackRadius(150.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f)
{
	InitHealth(GetMaxHealth());
}

void UFDCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UFDCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		FDGAS_LOG(LogFDGAS, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
	}
}
