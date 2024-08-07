// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_GASGameplayCue.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayCueManager.h"

FString UAnimNotify_GASGameplayCue::GetNotifyName_Implementation() const
{
	return TEXT("GASGameplayCue");
}

void UAnimNotify_GASGameplayCue::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
		if (OwnerActor && ASC)
		{
			FGameplayCueParameters CueParams;
			CueParams.Location = OwnerActor->GetActorLocation();
			//UAbilitySystemBlueprintLibrary::SendGameplayCue(OwnerActor, GameplayCueTag, CueParams);
			ASC->ExecuteGameplayCue(GameplayCueTag, CueParams);
		}
	}
}
