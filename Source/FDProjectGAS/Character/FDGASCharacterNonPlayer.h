// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FDCharacterNonPlayer.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "FDGASCharacterNonPlayer.generated.h"

DECLARE_DELEGATE_OneParam(FOnAbilityEndedDelegate, const UGameplayAbility*);

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

	virtual void AttackByAI() override;
	virtual void Deteced(bool isDeteced) override;

	UFUNCTION()
	virtual void OnOutOfHealth(AActor* MyInstigator);
	void ApplyGoldEffect(UAbilitySystemComponent* Target);

	UFUNCTION()
	void OnAbilityEnded(UGameplayAbility* Ability);

	void OnHealthChanged(const FOnAttributeChangeData& ChangeData);

protected:
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);



protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	UPROPERTY()
	TObjectPtr<class UFDCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> InitStatEffect;
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayEffect> GoldEffect;

	UPROPERTY(EditAnywhere, Category = GAS)
	float Level;

	//UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UFDGASWidgetComponent> HpBar;
};
