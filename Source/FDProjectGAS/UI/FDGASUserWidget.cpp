// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UFDGASUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UFDGASUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
