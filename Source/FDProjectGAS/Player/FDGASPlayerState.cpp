// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FDGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/FDCharacterAttributeSet.h"

AFDGASPlayerState::AFDGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	//ASC->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UFDCharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AFDGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
