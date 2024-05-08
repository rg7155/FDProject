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
	//��Ʈ�ѷ��� ���� ������ �� �߻��Ǵ� �̺�Ʈ
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
