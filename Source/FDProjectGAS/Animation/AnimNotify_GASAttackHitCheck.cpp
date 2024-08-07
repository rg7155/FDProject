// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_GASAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Character/FDCharacterPlayer.h"

UAnimNotify_GASAttackHitCheck::UAnimNotify_GASAttackHitCheck()
{
	ComboAttackLevel = 1.0f;
	bIsCameraShake = false;
	//UAnimNotify_GameplayCue
}

FString UAnimNotify_GASAttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASAttackHitCheck");
}

void UAnimNotify_GASAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			//GameplayEvent ���� �� �����Ϳ� ���� ���� �߰���
			//GA_AttackHitCheck �� ActivateAbility() ������ FGameplayEventData�� �Ѿ
			PayloadData.EventMagnitude = ComboAttackLevel;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}

		if (bIsCameraShake)
		{
			AFDCharacterPlayer* FDCharacterPlayer = Cast<AFDCharacterPlayer>(OwnerActor);
			if (FDCharacterPlayer)
			{
				FDCharacterPlayer->CameraShake();
			}
		}
	}
}
