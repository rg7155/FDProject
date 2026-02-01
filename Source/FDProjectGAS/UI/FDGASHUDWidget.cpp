// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASHUDWidget.h"
#include "UI/FDUserWidget.h"
#include "FDGASUserWidget.h"
#include "Character//FDGASCharacterPlayer.h"
#include "Blueprint/WidgetTree.h"


void UFDGASHUDWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	// 1. 위젯 트리를 순회하며 모든 자식 GAS 위젯에게 ASC 전파
	// (기존 NativeConstruct에 있던 로직을 여기로 이동)
	TArray<UWidget*> ChildWidgets;
	WidgetTree->GetAllWidgets(ChildWidgets);

	for (UWidget* Child : ChildWidgets)
	{
		UFDGASUserWidget* GASUserWidget = Cast<UFDGASUserWidget>(Child);
		if (GASUserWidget)
		{
			// 여기서 InOwner는 PlayerState 또는 Pawn이 들어옵니다.
			GASUserWidget->SetAbilitySystemComponent(InOwner);
		}
	}

	// 2. 캐릭터에게 "HUD가 생성되었음"을 알림
	// (ASC가 있다는 건 캐릭터도 존재한다는 뜻이므로 안전함)
	AFDGASCharacterPlayer* HUDPawn = Cast<AFDGASCharacterPlayer>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupGASHUDWidget(this);
	}
}

void UFDGASHUDWidget::ToggleShopVisible()
{
	if (WidgetShop)
	{
		WidgetShop->SetVisibility(WidgetShop->GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

void UFDGASHUDWidget::NativeConstruct()
{
	// 플레이어 컨트롤러 비긴플레이에서 AddToViewport 함수를 통해 실행됨
	Super::NativeConstruct();
}