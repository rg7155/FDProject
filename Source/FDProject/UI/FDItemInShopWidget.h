// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FDUserWidget.h"
#include "FDItemInShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDItemInShopWidget : public UFDUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnButtonPressed();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<class UFDItemData> ItemData;

	UPROPERTY()
	TObjectPtr<class UButton> ItemButton;
};
