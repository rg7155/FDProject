// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FDUserWidget.h"
#include "GameData/FDCharacterStat.h"
#include "FDHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDHpBarWidget : public UFDUserWidget
{
	GENERATED_BODY()

public:
	UFDHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FFDCharacterStat& BaseStat, const FFDCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
