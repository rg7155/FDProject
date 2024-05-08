// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "FDAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/FDCharacterAIInterface.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

//override
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//컨트롤하고 있는 폰 가져옴
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) 
		return EBTNodeResult::Failed;

	//네비 시스템 얻어옴, 버전업이 되면서 v1이 됐음
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem) 
		return EBTNodeResult::Failed;

	//해당 인터페이스 추후 구현
	IFDCharacterAIInterface* AIPawn = Cast<IFDCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn) 
		return EBTNodeResult::Failed;

	//블랙보드로 값 가져옴
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	float PatrolRadius = AIPawn->GetAIPatrolRadius();
	//네비로케이션
	FNavLocation NextPatrolPos;

	//홈포스, 반지름, 찾은값 넣어질 변수
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}