// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/FDGA_Attack.h"
#include "Character/FDCharacterBase.h"
#include "Character/FDGASCharacterNonPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "FDProjectGAS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/FDComboActionData.h"

UFDGA_Attack::UFDGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AttackSpeedRate = 1.0f;
}

void UFDGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AFDCharacterBase* FDCharacter = CastChecked<AFDCharacterBase>(ActorInfo->AvatarActor.Get());
	CurrentComboData = FDCharacter->GetComboActionData();
	FDCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), FDCharacter->GetComboActionMontage(), AttackSpeedRate);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UFDGA_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UFDGA_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	CurrentCombo = 1;

	StartComboTimer();
}

void UFDGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboInput = false;
	}
	else
	{
		HasNextComboInput = true;
	}
}

void UFDGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

}

void UFDGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AFDCharacterBase* FDCharacter = CastChecked<AFDCharacterBase>(ActorInfo->AvatarActor.Get());
	FDCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	CurrentComboData = nullptr;
	CurrentCombo = 0;
	HasNextComboInput = false;

	AFDGASCharacterNonPlayer* FDGASCharacterNonPlayer = Cast<AFDGASCharacterNonPlayer>(ActorInfo->AvatarActor.Get());
	if (FDGASCharacterNonPlayer)
	{
		FDGASCharacterNonPlayer->OnAbilityEnded(this);
	}
}

void UFDGA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UFDGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

FName UFDGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UFDGA_Attack::StartComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	//TODO
	const float ComboEffectiveTime = (CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate) / AttackSpeedRate;
	//FDGAS_LOG(LogFDGAS,)
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UFDGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UFDGA_Attack::CheckComboInput()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboInput)
	{
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextComboInput = false;
	}
}