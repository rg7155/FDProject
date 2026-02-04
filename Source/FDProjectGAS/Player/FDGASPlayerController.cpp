// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FDGASPlayerController.h"
#include "UI/FDGASHUDWidget.h"
#include "AbilitySystemComponent.h"

AFDGASPlayerController::AFDGASPlayerController()
{
	static ConstructorHelpers::FClassFinder<UFDGASHUDWidget> FDGASHUDWidgetRef(TEXT("/Game/FDPorjectGAS/UI/WBP_FDGASHUD.WBP_FDGASHUD_C"));
	if (FDGASHUDWidgetRef.Class)
	{
		FDGASHUDWidgetClass = FDGASHUDWidgetRef.Class;
	}
}

void AFDGASPlayerController::InitGASHUD(UAbilitySystemComponent* InASC)
{
	if (FDGASHUDWidget && InASC)
	{
		// [핵심] 이제 안전하게 위젯에 ASC를 주입
		// (참고: 위젯 클래스에 SetAbilitySystemComponent 같은 함수가 있어야 함)
		// 만약 위젯 함수 이름이 다르면 그에 맞게 수정하세요.
		FDGASHUDWidget->SetAbilitySystemComponent(InASC->GetAvatarActor());
	}
}

void AFDGASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		FInputModeGameOnly GameOnlyInputMode;
		SetInputMode(GameOnlyInputMode);

		FDGASHUDWidget = CreateWidget<UFDGASHUDWidget>(this, FDGASHUDWidgetClass);
		if (FDGASHUDWidget)
		{
			FDGASHUDWidget->AddToViewport();
		}
	}
}