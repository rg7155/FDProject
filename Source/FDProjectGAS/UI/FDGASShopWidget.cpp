// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASShopWidget.h"
#include "Engine/AssetManager.h" 
#include "Engine/StreamableManager.h" // FStreamableManager
#include "../Item/FDGASItemData.h" 

void UFDGASShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

    LoadPrimaryAssets();
}

void UFDGASShopWidget::LoadPrimaryAssets()
{
    UAssetManager& AssetManager = UAssetManager::Get();

    TArray<FPrimaryAssetId> AssetIds;
    AssetManager.GetPrimaryAssetIdList(FPrimaryAssetType("FDGASItemData"), AssetIds);

    for (const FPrimaryAssetId& AssetId : AssetIds)
    {
        // �񵿱� �ε�
        FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(AssetId);

        AssetManager.GetStreamableManager().RequestAsyncLoad(AssetPath,
            FStreamableDelegate::CreateLambda([=](){
              UObject* Object = AssetPath.ResolveObject();
              if (Object)
              {
                  UFDGASItemData* FDGASItemData = Cast<UFDGASItemData>(Object);
                  if (FDGASItemData)
                      K2_OnGASItemLoaded(FDGASItemData);
              }
          })
        );
    }
}

void UFDGASShopWidget::OnPrimaryAssetLoaded(FPrimaryAssetId AssetId, UObject* LoadedAsset)
{
    if (!LoadedAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load asset: %s"), *AssetId.ToString());
        return;
    }

    K2_OnGASItemLoaded(Cast<UFDGASItemData>(LoadedAsset));  // �������Ʈ���� ������ �̺�Ʈ ȣ��
}