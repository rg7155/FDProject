// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FDDamage.generated.h"

UCLASS()
class FDPROJECT_API AFDDamage : public AActor
{
	GENERATED_BODY()
	
public:	
	AFDDamage();

	void SetDamage(float DamageAmount);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFDWidgetComponent> DamageWidgetComponent;

	int RandomUpOffset = 0;
	int RandomRightOffset = 0;
};
