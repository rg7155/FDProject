// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FDItemData.generated.h"

//블프와 호환되도록
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
	Potion,
	Scroll
};

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("FDItemData", GetFName());
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int	Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString ItemName;
};
