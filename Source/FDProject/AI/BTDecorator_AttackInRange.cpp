// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"
#include "FDAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/FDCharacterAIInterface.h"
UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return false;

	IFDCharacterAIInterface* AIPawn = Cast<IFDCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn) return false;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target) return false;

	//타겟과 거리구하고 범위에 들어 왔으면
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	//Stat의 AttackRange 이용함
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();
	bResult = (DistanceToTarget <= AttackRangeWithRadius);
	return bResult;
}
