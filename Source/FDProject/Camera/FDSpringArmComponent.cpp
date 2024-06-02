// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/FDSpringArmComponent.h"

UFDSpringArmComponent::UFDSpringArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

FVector UFDSpringArmComponent::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime)
{
	return bHitSomething ? FMath::VInterpTo(GetComponentLocation(), TraceHitLocation, DeltaTime, 5.f) : DesiredArmLocation;
}
