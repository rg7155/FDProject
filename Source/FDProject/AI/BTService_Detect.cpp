// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "FDAI.h"
#include "AIController.h"
#include "Interface/FDCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/FDCollision.h"
#include "DrawDebugHelpers.h"
UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World) return;

	IFDCharacterAIInterface* AIPawn = Cast<IFDCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn) return;

	float DetectRadius = AIPawn->GetAIDetectRange();

	//������ �Լ��� ����Ͽ�
	//�÷��̾ �ټ� �ִٰ� �����Ͽ� ��Ƽ, �迭�� ����
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_FDACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			//���� �÷��̾� �϶���
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				//Ÿ�ٰ� ����
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				//�÷��̾� ��ġ�� ���� ������ ǥ��
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);

				AIPawn->Deteced(true);
				return;
			}
		}
	}
	//�÷��̾� �� ã���� Ÿ�� ��!!
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	AIPawn->Deteced(false);

}
