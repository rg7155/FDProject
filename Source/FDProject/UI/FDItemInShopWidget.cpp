// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDItemInShopWidget.h"
#include "Components/Button.h"
#include "Interface/FDCharacterItemInterface.h"
#include "FDProject.h"
#include "Item/FDItemData.h"

void UFDItemInShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("BtnItem")));
	ensure(ItemButton);
	//��������Ʈ ���� �� ���ε� �Լ� UFUNCTION �̿��� ��
	ItemButton->OnPressed.AddDynamic(this, &UFDItemInShopWidget::OnButtonPressed);
}

void UFDItemInShopWidget::OnButtonPressed()
{
	UE_LOG(LogFDProject, Log, TEXT("OnButtonPressed - %d"), ItemData->Type);
	IFDCharacterItemInterface* Pawn = Cast<IFDCharacterItemInterface>(GetOwningPlayerPawn());
	if (Pawn)
	{
		Pawn->TakeItem(ItemData);
	}
}
