// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/FDAnimationAttackInterface.h"

UAnimNotify_AttackHitCheck::UAnimNotify_AttackHitCheck()
{
	AttackMultiplier = 1.f;
}

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IFDAnimationAttackInterface* AttackPawn = Cast<IFDAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck(AttackMultiplier);
		}
	}
}