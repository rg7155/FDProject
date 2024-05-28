// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FDItemData.h"
#include "FDPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDPotionItemData : public UFDItemData
{
	GENERATED_BODY()
public:
	UFDPotionItemData();
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("FDItemData", GetFName());
	}
public:
	UPROPERTY(EditAnywhere, Category = Hp)
	float HealAmount;
};
