// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/FDCharacterWidgetInterface.h"
#include "FDProject.h"
#include "Components/TextBlock.h"

UFDHpBarWidget::UFDHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UFDHpBarWidget::NativeConstruct()
{
	//호출 시점은 UI관련 처리 끝났을때
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpProgressBar);

	IFDCharacterWidgetInterface* CharacterWidget = Cast<IFDCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UFDHpBarWidget::UpdateStat(const FFDCharacterStat& BaseStat, const FFDCharacterStat& ModifierStat)
{
	//캐릭터 베이스 SetupCharacterWidget 에서 함수 바인딩
	MaxHp = (BaseStat + ModifierStat).MaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void UFDHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp; 

	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		//이건 UMG 모듈에서 제공하므로 빌드설정에 "UMG" 추가 필수
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
		//FD_LOG(LogFDProject, Log, TEXT("%s"));
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UFDHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp);
}
