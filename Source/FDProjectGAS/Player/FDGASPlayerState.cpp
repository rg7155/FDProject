// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FDGASPlayerState.h"
#include "AbilitySystemComponent.h"

AFDGASPlayerState::AFDGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	//ASC->SetIsReplicated(true);
}

UAbilitySystemComponent* AFDGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
