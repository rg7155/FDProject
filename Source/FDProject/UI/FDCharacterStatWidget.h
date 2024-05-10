// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/FDCharacterStat.h"
#include "FDCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateStat(const FFDCharacterStat& BaseStat, const FFDCharacterStat& ModifierStat);
};
