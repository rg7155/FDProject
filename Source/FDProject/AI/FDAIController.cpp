// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FDAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/FDAI.h"

AFDAIController::AFDAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/FDProject/AI/BB_FDCharacter.BB_FDCharacter'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/FDProject/AI/BT_FDCharacter.BT_FDCharacter'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AFDAIController::RunAI()
{
	//Blackboard 기본 멤버 변수
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		//시작할 때 홈 포지션 값 추가함
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
		
		//성공했다면 BTAsset 구동
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AFDAIController::StopAI()
{
	//BrainComponent 기본 멤버 변수
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AFDAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}