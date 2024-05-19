// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDHUDWidget.h"
#include "Interface/FDCharacterHUDInterface.h"
#include "FDHpBarWidget.h"
#include "FDCharacterStatWidget.h"
#include "FDGoldWidget.h"

void UFDHUDWidget::UpdateStat(const FFDCharacterStat& BaseStat, const FFDCharacterStat& ModifierStat)
{
	FFDCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->UpdateStat(BaseStat, ModifierStat);
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UFDHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UFDHUDWidget::UpdateGold(int _Gold)
{
	Gold->UpdateGoldText(_Gold);
}

void UFDHUDWidget::SetShopVisible()
{
	Shop->SetVisibility(Shop->GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UFDHUDWidget::NativeConstruct()
{
	// 플레이어 컨트롤러 비긴플레이에서 AddToViewport 함수를 통해 실행됨
	Super::NativeConstruct();

	//WBP에서 변수들 이름 바꾸고 이걸로 가져옴
	HpBar = Cast<UFDHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UFDCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	Gold = Cast<UFDGoldWidget>(GetWidgetFromName(TEXT("WidgetGold")));
	ensure(Gold);

	Shop = Cast<UFDUserWidget>(GetWidgetFromName(TEXT("WidgetShop")));
	ensure(Shop);

	IFDCharacterHUDInterface* HUDPawn = Cast<IFDCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}

}