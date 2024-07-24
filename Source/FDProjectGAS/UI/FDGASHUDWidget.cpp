// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASHUDWidget.h"
#include "FDGASHpBarUserWidget.h"
#include "FDGASGoldWidget.h"
#include "FDGASSkillIconWidget.h"
#include "UI/FDUserWidget.h"
#include "Character//FDGASCharacterPlayer.h"


void UFDGASHUDWidget::ToggleShopVisible()
{
	isShopVisible = !isShopVisible;
	if (isShopVisible)
	{
		WidgetShop->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WidgetShop->SetVisibility(ESlateVisibility::Hidden);
	}
	//Shop->SetVisibility(Shop->GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UFDGASHUDWidget::NativeConstruct()
{
	// �÷��̾� ��Ʈ�ѷ� ����÷��̿��� AddToViewport �Լ��� ���� �����
	Super::NativeConstruct();

	WidgetGASHpBar->SetAbilitySystemComponent(GetOwningPlayerPawn());
	WidgetGold->SetAbilitySystemComponent(GetOwningPlayerPawn());
	WidgetSkillIcon->SetAbilitySystemComponent(GetOwningPlayerPawn());

	AFDGASCharacterPlayer* HUDPawn = Cast<AFDGASCharacterPlayer>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupGASHUDWidget(this);
	}

}