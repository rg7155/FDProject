// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "FDTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API AFDTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	AFDTA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;
	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
};
