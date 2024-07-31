// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASHUDWidget.h"
#include "UI/FDUserWidget.h"
#include "FDGASUserWidget.h"
#include "Character//FDGASCharacterPlayer.h"
#include "Blueprint/WidgetTree.h"


void UFDGASHUDWidget::ToggleShopVisible()
{
	//TODO GA�� �ٲٱ�
	//isShopVisible = !isShopVisible;
	//if (isShopVisible)
	//{
	//	WidgetShop->SetVisibility(ESlateVisibility::Visible);
	//}
	//else
	//{
	//	WidgetShop->SetVisibility(ESlateVisibility::Hidden);
	//}
	WidgetShop->SetVisibility(WidgetShop->GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UFDGASHUDWidget::NativeConstruct()
{
	// �÷��̾� ��Ʈ�ѷ� ����÷��̿��� AddToViewport �Լ��� ���� �����
	Super::NativeConstruct();

	TArray<UWidget*> ChildWidgets;
	WidgetTree->GetAllWidgets(ChildWidgets);
	for (UWidget* Child : ChildWidgets)
	{
		UFDGASUserWidget* GASUserWidget = Cast<UFDGASUserWidget>(Child);
		if (GASUserWidget)
		{
			GASUserWidget->SetAbilitySystemComponent(GetOwningPlayerPawn());
		}
	}

	//WidgetGASHpBar->SetAbilitySystemComponent(GetOwningPlayerPawn());
	//WidgetGold->SetAbilitySystemComponent(GetOwningPlayerPawn());
	//WidgetSkillIcon->SetAbilitySystemComponent(GetOwningPlayerPawn());
	//WidgetGASCostBar->SetAbilitySystemComponent(GetOwningPlayerPawn());

	AFDGASCharacterPlayer* HUDPawn = Cast<AFDGASCharacterPlayer>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupGASHUDWidget(this);
	}

}