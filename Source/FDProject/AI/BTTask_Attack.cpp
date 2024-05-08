// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/FDCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	IFDCharacterAIInterface* AIPawn = Cast<IFDCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn) return EBTNodeResult::Failed;

	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
	{
		//InProgress중 성공했다고 알려주는 함수
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAI();
	//공격이 끝난 이후에 Succeeded로 변경
	return EBTNodeResult::InProgress;
}
