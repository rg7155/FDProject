// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/FDCharacterWidgetInterface.h"
#include "FDProject.h"

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

	IFDCharacterWidgetInterface* CharacterWidget = Cast<IFDCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UFDHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		//이건 UMG 모듈에서 제공하므로 빌드설정에 "UMG" 추가 필수
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
		//FD_LOG(LogFDProject, Log, TEXT("%s"));
	}
}
