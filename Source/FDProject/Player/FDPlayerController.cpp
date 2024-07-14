// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FDPlayerController.h"
#include "UI/FDHUDWidget.h"

AFDPlayerController::AFDPlayerController()
{

}

void AFDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void AFDPlayerController::ToggleMouseCursor(bool isShowCursor)
{
	if (isShowCursor)
	{
		bShowMouseCursor = true;
		//SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		bShowMouseCursor = false;
		//SetInputMode(FInputModeGameOnly());
	}
}