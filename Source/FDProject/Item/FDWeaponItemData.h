// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FDItemData.h"
#include "GameData/FDCharacterStat.h"
#include "FDWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDWeaponItemData : public UFDItemData
{
	GENERATED_BODY()
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("FDItemData", GetFName());
	}
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FFDCharacterStat BaseStat;
};
