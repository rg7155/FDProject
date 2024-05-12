// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FDUserWidget.h"
#include "FDDamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API UFDDamageWidget : public UFDUserWidget
{
	GENERATED_BODY()
	
public:
	UFDDamageWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;

public:
	void SetDamageText(float Damage);


protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> DamageText;

	float Damage = 0.0f;
};
