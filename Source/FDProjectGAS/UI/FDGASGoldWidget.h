// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FDGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "FDGASGoldWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDGASGoldWidget : public UFDGASUserWidget
{
	GENERATED_BODY()
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

protected:
	virtual void OnGoldChanged(const FOnAttributeChangeData& ChangeData);

	void UpdateGoldText();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtGold;

	int Gold = 0;
};
