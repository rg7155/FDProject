// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/FDGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/FDAT_Trace.h"
#include "GA/TA/FDTA_Trace.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "FDProjectGAS.h"

UFDGA_AttackHitCheck::UFDGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFDGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UFDAT_Trace* AttackTraceTask = UFDAT_Trace::CreateTask(this, AFDTA_Trace::StaticClass());
	AttackTraceTask->OnComplete.AddDynamic(this, &UFDGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}
void UFDGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		//ABGAS_LOG(LogABGAS, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (!SourceASC || !TargetASC)
		{
			FDGAS_LOG(LogFDGAS, Error, TEXT("ASC not found!"));
			return;
		}

		const UFDCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UFDCharacterAttributeSet>();
		UFDCharacterAttributeSet* TargetAttribute = const_cast<UFDCharacterAttributeSet*>(TargetASC->GetSet<UFDCharacterAttributeSet>());
		if (!SourceAttribute || !TargetAttribute)
		{
			FDGAS_LOG(LogFDGAS, Error, TEXT("ASC not found!"));
			return;
		}

		const float AttackDamage = SourceAttribute->GetAttackRate();
		TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);
	}

	//TODO

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
