// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FDGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "FDGASCostBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDGASCostBarWidget : public UFDGASUserWidget
{
	GENERATED_BODY()
public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
	void OnCostChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxCostChanged(const FOnAttributeChangeData& ChangeData);

protected:
	void SetCostBar();

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> SkillCostRecoveryEffect;
	FActiveGameplayEffectHandle SkillCostRecoveryEffectHandle;
	bool isSkillCostRecoveryEffectRemoved = false;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbCostBar;

	float CurrentCost = 0.f;
	float MaxCost = 0.f;
};
