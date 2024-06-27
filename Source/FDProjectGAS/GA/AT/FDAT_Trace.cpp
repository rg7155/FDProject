// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/FDAT_Trace.h"
#include "GA/TA/FDTA_Trace.h"
#include "AbilitySystemComponent.h"

UFDAT_Trace::UFDAT_Trace()
{
}

UFDAT_Trace* UFDAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AFDTA_Trace> TargetActorClass)
{
	UFDAT_Trace* NewTask = NewAbilityTask<UFDAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UFDAT_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UFDAT_Trace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UFDAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AFDTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UFDAT_Trace::OnTargetDataReadyCallback);
	}
}

void UFDAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		//ASC���� ������ �� �ֵ��� ���
		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		//GA�� TA���� �Ѱ���
		SpawnedTargetActor->StartTargeting(Ability);
		//�̶� TA�� ConfirmTargetingAndContinue �Լ� ���� ��.
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UFDAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}