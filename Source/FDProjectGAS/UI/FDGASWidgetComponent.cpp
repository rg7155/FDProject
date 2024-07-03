// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASWidgetComponent.h"
#include "UI/FDGASUserWidget.h"

void UFDGASWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UFDGASUserWidget* GASUserWidget = Cast<UFDGASUserWidget>(GetWidget());
	if (GASUserWidget)
	{
		GASUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}
