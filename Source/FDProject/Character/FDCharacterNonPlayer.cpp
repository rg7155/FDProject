// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDCharacterNonPlayer.h"
#include "AI/FDAIController.h"
#include "Character/FDCharacterStatComponent.h"

AFDCharacterNonPlayer::AFDCharacterNonPlayer()
{
	AIControllerClass = AFDAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
float AFDCharacterNonPlayer::GetAIPatrolRadius() { return 800.0f; }
float AFDCharacterNonPlayer::GetAIDetectRange() { return 400.0f; }
float AFDCharacterNonPlayer::GetAIAttackRange()
{
	//�̸� ���� ���� Stat�� �߰�
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}
float AFDCharacterNonPlayer::GetAITurnSpeed() { return 3.0f; }

void AFDCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AFDCharacterNonPlayer::AttackByAI()
{
	//�÷��̾�� �Է� ���������� �����ϰ� �Լ� ȣ����
	ProcessComboCommand();
}

void AFDCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
