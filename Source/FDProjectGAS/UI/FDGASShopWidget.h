// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FDGASUserWidget.h"
//#include "Item/FDGASItemData.h"
#include "FDGASShopWidget.generated.h"

/**
 * 
 */
class UFDGASItemData;

UCLASS()
class FDPROJECTGAS_API UFDGASShopWidget : public UFDGASUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

private:
	void LoadPrimaryAssets();
	void OnPrimaryAssetLoaded(FPrimaryAssetId AssetId, UObject* LoadedAsset);

public:
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnGASItemLoaded"))
	void K2_OnGASItemLoaded(UFDGASItemData* GASItemData);
};
