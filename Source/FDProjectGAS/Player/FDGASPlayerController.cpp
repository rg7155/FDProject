// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FDGASPlayerController.h"
#include "UI/FDGASHUDWidget.h"

AFDGASPlayerController::AFDGASPlayerController()
{
	static ConstructorHelpers::FClassFinder<UFDGASHUDWidget> FDGASHUDWidgetRef(TEXT("/Game/FDPorjectGAS/UI/WBP_FDGASHUD.WBP_FDGASHUD_C"));
	if (FDGASHUDWidgetRef.Class)
	{
		FDGASHUDWidgetClass = FDGASHUDWidgetRef.Class;
	}
}

void AFDGASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	FDGASHUDWidget = CreateWidget<UFDGASHUDWidget>(this, FDGASHUDWidgetClass);
	if (FDGASHUDWidget)
	{
		FDGASHUDWidget->AddToViewport();
	}
}