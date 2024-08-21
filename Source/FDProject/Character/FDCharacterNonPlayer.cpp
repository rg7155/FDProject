// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDCharacterNonPlayer.h"
#include "AI/FDAIController.h"

AFDCharacterNonPlayer::AFDCharacterNonPlayer()
{
	AIControllerClass = AFDAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PatrolRadius = 800.f;
	DetectRange = 400.f;
	AttackRange = 200.f;
}

void AFDCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	AFDAIController* FDAIController = Cast<AFDAIController>(GetController());
	if (FDAIController)
	{
		FDAIController->StopAI();
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
	{
		Destroy();
	}
	), DeadEventDelayTime, false);
}
float AFDCharacterNonPlayer::GetAIPatrolRadius() 
{ 
	return PatrolRadius;
}
float AFDCharacterNonPlayer::GetAIDetectRange() 
{ 
	return DetectRange;
}
float AFDCharacterNonPlayer::GetAIAttackRange()
{
	return AttackRange;
	//return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}
float AFDCharacterNonPlayer::GetAITurnSpeed()
{
	return 3.0f; 
}

void AFDCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AFDCharacterNonPlayer::AttackByAI()
{
	//플레이어에서 입력 들어왔을때와 동일하게 함수 호출함
	//ProcessComboCommand();
}

void AFDCharacterNonPlayer::Deteced(bool isDeteced)
{
}



