// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FDCharacterBase.h"
#include "Interface/FDCharacterAIInterface.h"
#include "FDCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API AFDCharacterNonPlayer : public AFDCharacterBase , public IFDCharacterAIInterface
{
	GENERATED_BODY()
public:
	AFDCharacterNonPlayer();

protected:
	void SetDead() override;

	// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;
	virtual void Deteced(bool isDeteced) override;

	FAICharacterAttackFinished OnAttackFinished;
};
