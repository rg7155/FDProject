// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FDGASHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDGASHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void ToggleShopVisible();

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFDGASHpBarUserWidget> WidgetGASHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFDGASGoldWidget> WidgetGold;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFDUserWidget> WidgetShop;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFDGASSkillIconWidget> WidgetSkillIcon;

	bool isShopVisible = false;
};
