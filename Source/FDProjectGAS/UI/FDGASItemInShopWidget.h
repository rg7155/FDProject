// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FDGASUserWidget.h"
#include "FDGASItemInShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDGASItemInShopWidget : public UFDGASUserWidget
{
	GENERATED_BODY()
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonPressed();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<class UFDGASItemData> ItemData;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> btnItem;
};
