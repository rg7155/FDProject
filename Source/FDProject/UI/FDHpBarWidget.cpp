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
	//ȣ�� ������ UI���� ó�� ��������
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
		//�̰� UMG ��⿡�� �����ϹǷ� ���弳���� "UMG" �߰� �ʼ�
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
		//FD_LOG(LogFDProject, Log, TEXT("%s"));
	}
}
