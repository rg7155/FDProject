// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FDCharacterBase.h"
#include "FDCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API AFDCharacterNonPlayer : public AFDCharacterBase
{
	GENERATED_BODY()
public:
	AFDCharacterNonPlayer();

protected:
	void SetDead() override;
};
