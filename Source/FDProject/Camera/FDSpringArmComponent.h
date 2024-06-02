// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "FDSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FDPROJECT_API UFDSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	UFDSpringArmComponent();

protected:
	virtual FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime) override;
	
};
