// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/TA/FDTA_Trace.h"
#include "FDTA_SphereMultiTrace.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API AFDTA_SphereMultiTrace : public AFDTA_Trace
{
	GENERATED_BODY()

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
