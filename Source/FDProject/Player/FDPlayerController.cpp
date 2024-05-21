// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FDPlayerController.h"
#include "UI/FDHUDWidget.h"

AFDPlayerController::AFDPlayerController()
{
	static ConstructorHelpers::FClassFinder<UFDHUDWidget> FDHUDWidgetRef(TEXT("/Game/FDProject/UI/WBP_FDHUD.WBP_FDHUD_C"));
	if (FDHUDWidgetRef.Class)
	{
		FDHUDWidgetClass = FDHUDWidgetRef.Class;
	}
}

void AFDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	FDHUDWidget = CreateWidget<UFDHUDWidget>(this, FDHUDWidgetClass);
	if (FDHUDWidget)
	{
		FDHUDWidget->AddToViewport();
	}
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