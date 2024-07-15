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
	void SetShopVisible(bool isVisible);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UFDGASHpBarUserWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UFDGASGoldWidget> Gold;

	UPROPERTY()
	TObjectPtr<class UFDUserWidget> Shop;
};
