// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASItemInShopWidget.h"
#include "Components/Button.h"
#include "FDProjectGAS.h"
#include "Interface/FDGASCharacterItemInterface.h"
#include "Item/FDGASItemData.h"

void UFDGASItemInShopWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
}

void UFDGASItemInShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(btnItem);
	//델리게이트 설정 시 바인딩 함수 UFUNCTION 이여야 함
	btnItem->OnPressed.AddDynamic(this, &UFDGASItemInShopWidget::OnButtonPressed);
}

void UFDGASItemInShopWidget::OnButtonPressed()
{
	IFDGASCharacterItemInterface* Pawn = Cast<IFDGASCharacterItemInterface>(GetOwningPlayerPawn());
	if (Pawn)
		Pawn->TakeGASItem(ItemData);
}
