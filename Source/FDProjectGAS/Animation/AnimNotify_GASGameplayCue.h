// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayCueInterface.h"
#include "AnimNotify_GASGameplayCue.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UAnimNotify_GASGameplayCue : public UAnimNotify
{
	GENERATED_BODY()
protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayCue, meta = (Categories = "GameplayCue"))
	FGameplayTag GameplayCueTag;
};
