// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FDGASUserWidget.h"
#include "FDGASHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDGASHUDWidget : public UFDGASUserWidget
{
	GENERATED_BODY()
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	void ToggleShopVisible();

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFDGASHpBarUserWidget> WidgetGASHpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFDGASGoldWidget> WidgetGold;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFDGASUserWidget> WidgetShop;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFDGASSkillIconWidget> WidgetSkillIcon;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UFDGASCostBarWidget> WidgetGASCostBar;

	bool isShopVisible = false;
};
