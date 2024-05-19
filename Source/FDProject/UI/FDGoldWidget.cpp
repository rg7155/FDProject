// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGoldWidget.h"
#include "Components/TextBlock.h"

UFDGoldWidget::UFDGoldWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UFDGoldWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GoldText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtGold")));
	ensure(GoldText);
}

void UFDGoldWidget::UpdateGoldText(int Gold)
{
	if (GoldText)
	{
		//GoldText->SetText(FText::AsNumber(Gold));
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d Gold"), Gold)));
	}
}