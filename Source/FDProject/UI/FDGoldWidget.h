// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FDUserWidget.h"
#include "FDGoldWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDGoldWidget : public UFDUserWidget
{
	GENERATED_BODY()
public:
	UFDGoldWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
public:
	void UpdateGoldText(int Gold);

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> GoldText;
};

