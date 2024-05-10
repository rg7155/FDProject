// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDHUDWidget.h"
//#include "Interface/FDCharacterHUDInterface.h"
#include "FDHpBarWidget.h"
#include "FDCharacterStatWidget.h"

void UFDHUDWidget::UpdateStat(const FFDCharacterStat& BaseStat, const FFDCharacterStat& ModifierStat)
{
	FFDCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->SetMaxHp(TotalStat.MaxHp);

	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UFDHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UFDHUDWidget::NativeConstruct()
{
	// �÷��̾� ��Ʈ�ѷ� ����÷��̿��� AddToViewport �Լ��� ���� �����
	Super::NativeConstruct();

	//WBP���� ������ �̸� �ٲٰ� �̰ɷ� ������
	HpBar = Cast<UFDHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UFDCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	//IFDCharacterHUDInterface* HUDPawn = Cast<IFDCharacterHUDInterface>(GetOwningPlayerPawn());
	//if (HUDPawn)
	//{
	//	HUDPawn->SetupHUDWidget(this);
	//}

}