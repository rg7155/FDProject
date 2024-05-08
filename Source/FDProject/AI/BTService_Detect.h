// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Detect();

protected:
	//오버라이드, 지정한 인터벌마다 실행됨
	//부모 클래스 생성자에 기본값 설정
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
