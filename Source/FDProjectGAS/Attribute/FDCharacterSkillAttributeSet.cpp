// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/FDCharacterSkillAttributeSet.h"

UFDCharacterSkillAttributeSet::UFDCharacterSkillAttributeSet() :
	SkillRange(800.0f),
	MaxSkillRange(1200.0f),
	SkillAttackRate(150.0f),
	MaxSkillAttackRate(300.0f),
	SkillEnergy(100.0f),
	MaxSkillEnergy(100.0f),
	SkillCooldown(3.0f),
	SkillRecovery(1.0f)
{
}

void UFDCharacterSkillAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	SetClampData(Attribute, NewValue);
}

void UFDCharacterSkillAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	SetClampData(Attribute, NewValue);
}

void UFDCharacterSkillAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	OnChanged.Broadcast();
}

void UFDCharacterSkillAttributeSet::SetClampData(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetSkillRangeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, GetMaxSkillRange());
	}
	else if (Attribute == GetSkillAttackRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSkillAttackRate());
	}
	else if (Attribute == GetSkillEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxSkillEnergy());
	}
}
