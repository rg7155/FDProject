// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FDPlayerController.h"

void AFDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
