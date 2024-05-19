// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/FDCharacterStat.h"
#include "FDHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateStat(const FFDCharacterStat& BaseStat, const FFDCharacterStat& ModifierStat);
	//ABHpBar와 동일함
	void UpdateHpBar(float NewCurrentHp);
	void UpdateGold(int Gold);

	void SetShopVisible();

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UFDHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UFDCharacterStatWidget> CharacterStat;

	UPROPERTY()
	TObjectPtr<class UFDGoldWidget> Gold;

	UPROPERTY()
	TObjectPtr<class UFDUserWidget> Shop;
};
