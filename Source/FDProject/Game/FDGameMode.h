// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FDGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API AFDGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFDGameMode();

	void OnPlayerDead();
};
