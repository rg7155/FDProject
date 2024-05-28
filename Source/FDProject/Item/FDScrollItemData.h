// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FDItemData.h"
#include "GameData/FDCharacterStat.h"
#include "FDScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDScrollItemData : public UFDItemData
{
	GENERATED_BODY()
public:
	UFDScrollItemData();
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("FDItemData", GetFName());
	}
public:
	UPROPERTY(EditAnywhere, Category = Stat)
	FFDCharacterStat BaseStat;
};
