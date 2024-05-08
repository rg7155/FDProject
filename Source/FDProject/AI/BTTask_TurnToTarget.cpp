// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "FDAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/FDCharacterAIInterface.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn) return EBTNodeResult::Failed;

	IFDCharacterAIInterface* AIPawn = Cast<IFDCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn) return EBTNodeResult::Failed;

	float TurnSpeed = AIPawn->GetAITurnSpeed(); //3.0
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	//스피드에 의해 서서히 돌아가도록
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}
