// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FDCharacterNonPlayer.h"
#include "AbilitySystemInterface.h"
#include "FDGASCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API AFDGASCharacterNonPlayer : public AFDCharacterNonPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AFDGASCharacterNonPlayer();
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION()
	virtual void OnOutOfHealth(AActor* MyInstigator);
	void ApplyGoldEffect(UAbilitySystemComponent* Target);

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY()
	TObjectPtr<class UFDCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> GoldEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	float Level;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UFDGASWidgetComponent> HpBar;
};
