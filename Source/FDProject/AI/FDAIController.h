// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FDAIController.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API AFDAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AFDAIController();

	void RunAI();
	void StopAI();

protected:
	//컨트롤러가 폰에 빙의할 때 발생되는 이벤트
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
