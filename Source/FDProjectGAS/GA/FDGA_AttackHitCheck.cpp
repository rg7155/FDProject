// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/FDGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/FDAT_Trace.h"
#include "GA/TA/FDTA_Trace.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "FDProjectGAS.h"
#include "Tag/FDGameplayTag.h"

UFDGA_AttackHitCheck::UFDGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFDGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentLevel = TriggerEventData->EventMagnitude;
	FDGAS_LOG(LogFDGAS, Log, TEXT("FDGA_AttackHitCheck ActivateAbility "));

	UFDAT_Trace* AttackTraceTask = UFDAT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UFDGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UFDGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0) || 
		UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		FDGAS_LOG(LogFDGAS, Log, TEXT("FDGA_AttackHitCheck TargetDataHasHitResult "));
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		const UFDCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UFDCharacterAttributeSet>();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			//1.현재 어빌 스펙 핸들 4.이펙트 핸들 5.콜백으로 받은 타겟 데이터 핸들
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);


		/*	FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;*/
		}

		//TArray<AActor*> TargetActors = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData()
	}


	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
