// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FDGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDGA_Attack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFDGA_Attack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	UFUNCTION()
	void OnInterruptedCallback();
	
	FName GetNextSection();
	FName GetRandomSection();
	FName GetSection(uint8 Index);

	void StartComboTimer();
	void CheckComboInput();

protected:
	UPROPERTY()
	TObjectPtr<class UFDComboActionData> CurrentComboData;

	UPROPERTY(EditAnywhere)
	float AttackSpeedRate;

	uint8 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboInput = false;
};
