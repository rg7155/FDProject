// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDCharacterNonPlayer.h"

AFDCharacterNonPlayer::AFDCharacterNonPlayer()
{
}

void AFDCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
	{
		Destroy();
	}
	), DeadEventDelayTime, false);
}
