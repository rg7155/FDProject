// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API AFDPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AFDPlayerController();
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ToggleMouseCursor();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver(); 
	
	void GameOver();


};
