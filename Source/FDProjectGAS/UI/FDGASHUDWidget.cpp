// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASHUDWidget.h"
#include "FDGASHpBarUserWidget.h"
#include "FDGASGoldWidget.h"
#include "UI/FDUserWidget.h"
#include "Character//FDGASCharacterPlayer.h"


void UFDGASHUDWidget::SetShopVisible(bool isVisible)
{
	if (isVisible)
	{
		Shop->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Shop->SetVisibility(ESlateVisibility::Hidden);
	}
	//Shop->SetVisibility(Shop->GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UFDGASHUDWidget::NativeConstruct()
{
	// 플레이어 컨트롤러 비긴플레이에서 AddToViewport 함수를 통해 실행됨
	Super::NativeConstruct();

	//WBP에서 변수들 이름 바꾸고 이걸로 가져옴
	HpBar = Cast<UFDGASHpBarUserWidget>(GetWidgetFromName(TEXT("WidgetGASHpBar")));
	HpBar->SetAbilitySystemComponent(GetOwningPlayerPawn());
	ensure(HpBar);

	Gold = Cast<UFDGASGoldWidget>(GetWidgetFromName(TEXT("WidgetGold")));
	Gold->SetAbilitySystemComponent(GetOwningPlayerPawn());
	ensure(Gold);

	Shop = Cast<UFDUserWidget>(GetWidgetFromName(TEXT("WidgetShop")));
	ensure(Shop);
	SetShopVisible(false);

	AFDGASCharacterPlayer* HUDPawn = Cast<AFDGASCharacterPlayer>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupGASHUDWidget(this);
	}

}