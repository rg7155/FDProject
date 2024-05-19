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
	ItemButton->OnPressed.AddDynamic(this, &UFDItemInShopWidget::OnButtonPressed);
}

void UFDItemInShopWidget::OnButtonPressed()
{
	//델리게이트 설정 시 바인딩 함수 UFUNCTION
	UE_LOG(LogFDProject, Log, TEXT("OnButtonPressed - %d"), ItemData->Type);
	//
	/*IFDCharacterItemInterface* Pawn = Cast<IFDCharacterItemInterface>(OtherActor);
	if (Pawn)
	{
		Pawn->TakeItem(ItemData);
	}*/
}
