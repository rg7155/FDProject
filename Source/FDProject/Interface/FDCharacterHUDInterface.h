// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FDCharacterHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFDCharacterHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FDPROJECT_API IFDCharacterHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetupHUDWidget(class UFDHUDWidget* InHUDWidget) = 0;
};
