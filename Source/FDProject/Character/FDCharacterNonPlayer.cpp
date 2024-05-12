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
float AFDCharacterNonPlayer::GetAIPatrolRadius() 
{ 
	return 800.0f; 
}
float AFDCharacterNonPlayer::GetAIDetectRange() 
{ 
	return 400.0f; 
}
float AFDCharacterNonPlayer::GetAIAttackRange()
{
	//�̸� ���� ���� Stat�� �߰�
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
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
	//�÷��̾�� �Է� ���������� �����ϰ� �Լ� ȣ����
	ProcessComboCommand();
}

void AFDCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}

float AFDCharacterNonPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
	//AActor* DamageActor = GetWorld()->SpawnActor(AFDDamage::StaticClass(), &SpawnLocation, &FRotator::ZeroRotator);
	//AFDDamage* FDDamageActor = Cast<AFDDamage>(DamageActor);
	//if (FDDamageActor)
	//{
	//	FDDamageActor->SetDamage(DamageAmount);
	//	//FDDamageActor->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnOpponentDestroyed);
	//}

	return DamageAmount;
}
