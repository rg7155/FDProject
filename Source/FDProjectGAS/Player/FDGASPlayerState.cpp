// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FDGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "Attribute/FDCharacterSkillAttributeSet.h"

AFDGASPlayerState::AFDGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	//ASC->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UFDCharacterAttributeSet>(TEXT("AttributeSet"));
	SkillAttributeSet = CreateDefaultSubobject<UFDCharacterSkillAttributeSet>(TEXT("SkillAttributeSet"));
}

UAbilitySystemComponent* AFDGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
