// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FDGASUserWidget.h"
#include "GameplayTagContainer.h"
#include "FDGASSkillIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDGASSkillIconWidget : public UFDGASUserWidget
{
	GENERATED_BODY()

public:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
public:

	UFUNCTION(BlueprintImplementableEvent, Category = UI, Meta = (DisplayName = "OnPlayCooldownAnimCpp"))
	void K2_OnPlayCooldownAnim(float Cooldown);

	void PlayCooldownAnim(float Cooldown);

protected:
	void OnSkillCooldownTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbCooldown;

};
