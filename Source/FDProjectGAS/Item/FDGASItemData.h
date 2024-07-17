// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FDGASItemData.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDGASItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("FDGASItemData", GetFName());
	}
public:
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> ItemEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString ItemName;

	UPROPERTY(EditAnywhere, Category = GAS)
	float Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int	Gold;
};
