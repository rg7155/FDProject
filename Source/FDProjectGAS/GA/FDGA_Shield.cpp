// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/FDGA_Shield.h"
#include "Character/FDCharacterPlayer.h"
#include "Animation/FDAnimInstance.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
//#include "ArenaBattleGAS.h"
#include "GameFramework/CharacterMovementComponent.h"

UFDGA_Shield::UFDGA_Shield()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFDGA_Shield::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AFDCharacterPlayer* FDCharacter = CastChecked<AFDCharacterPlayer>(ActorInfo->AvatarActor.Get());

	SetAnimInstanceShield(ActorInfo, true);
	ApplyGameEffectShield(Handle, ActorInfo, ActivationInfo, true);

	UAbilityTask_PlayMontageAndWait* PlayShieldTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayShield"), FDCharacter->GetShieldMontage());
	PlayShieldTask->ReadyForActivation();
}

void UFDGA_Shield::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UFDGA_Shield::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UFDGA_Shield::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	SetAnimInstanceShield(ActorInfo, false);
	ApplyGameEffectShield(Handle, ActorInfo, ActivationInfo, false); 
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFDGA_Shield::SetAnimInstanceShield(const FGameplayAbilityActorInfo* ActorInfo, bool isOn)
{
	AFDCharacterPlayer* FDCharacter = CastChecked<AFDCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UFDAnimInstance* AnimInstance = Cast<UFDAnimInstance>(FDCharacter->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetShield(isOn);
	}
}

void UFDGA_Shield::ApplyGameEffectShield(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool isOn)
{
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(isOn ? ShieldOnEffect : ShieldOffEffect);
	if (EffectSpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, EffectSpecHandle);
	}
}

