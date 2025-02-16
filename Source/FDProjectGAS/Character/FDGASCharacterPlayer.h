// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FDCharacterPlayer.h"
#include "AbilitySystemInterface.h"
#include "Interface/FDGASCharacterItemInterface.h"
#include "FDGASCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API AFDGASCharacterPlayer : public AFDCharacterPlayer, public IAbilitySystemInterface, public IFDGASCharacterItemInterface
{
	GENERATED_BODY()

public:
	AFDGASCharacterPlayer();
	UFUNCTION()
	virtual void OnOutOfHealth(AActor* MyInstigator);

	virtual void Respawn() override;
	 

	FORCEINLINE virtual class UAnimMontage* GetSkillActionMontage() const { return SkillActionMontage; }

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Interaction() override;

	virtual void TakeGASItem(class UFDGASItemData* InItemData) override;

	void SetupGASHUDWidget(class UFDGASHUDWidget* InHUDWidget);



protected:
	void SetupGASInputComponent();
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> SkillActionMontage;

protected:
	UFDGASHUDWidget* GASHUDWidget;
};
