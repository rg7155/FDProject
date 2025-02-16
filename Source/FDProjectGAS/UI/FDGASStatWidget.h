// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FDGASUserWidget.h"
#include "FDGASStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDGASStatWidget : public UFDGASUserWidget
{
	GENERATED_BODY()
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;

	void AddLockUp(const FString& PropName);

	UFUNCTION()
	void OnUpdateStat();

	UFUNCTION()
	void OnUpdateSkillStat();

	void UpdateStat();
	void UpdateSkillStat();

	void UpdateStatText(const FName& PropKey, float BaseValue, float CurrentValue);

private:
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;
	TMap<FName, class UTextBlock*> ModifierLookup;
};
