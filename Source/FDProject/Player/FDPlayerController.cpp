// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FDPlayerController.h"
#include "UI/FDHUDWidget.h"

AFDPlayerController::AFDPlayerController()
{
	bShowMouseCursor = false;
}

void AFDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void AFDPlayerController::ToggleMouseCursor()
{
	bShowMouseCursor = !bShowMouseCursor;
	if (bShowMouseCursor)
	{
		SetInputMode(FInputModeUIOnly());
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}
}