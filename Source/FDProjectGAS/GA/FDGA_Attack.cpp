// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/FDGA_Attack.h"
#include "Character/FDCharacterBase.h"
#include "Character/FDGASCharacterNonPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "FDProjectGAS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/FDComboActionData.h"
#include "AbilitySystemComponent.h"

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

	// 첫 번째 섹션 이름을 가져옵니다 (ComboAttack1)
	FName StartSectionName = GetSection(CurrentCombo);
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, StartSectionName, FDCharacter->GetComboActionMontage(), AttackSpeedRate);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UFDGA_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UFDGA_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	AFDGASCharacterNonPlayer* FDGASCharacterNonPlayer = Cast<AFDGASCharacterNonPlayer>(ActorInfo->AvatarActor.Get());
	if (FDGASCharacterNonPlayer)
	{
		MontageJumpToSection(GetRandomSection());
	}
	else
	{
		CurrentCombo = 1;
		StartComboTimer();
	}
	//FDGAS_LOG(LogFDGAS, Log, TEXT("%d"), CurrentCombo);

	if (FDCharacter)
	{
		// 다음 틱 기다리지 말고 지금 당장 상태값(몽타주, 태그) 쏴라
		// 이걸 호출하면 대기열 무시하고 즉시 리플리케이션 수행함
		if (FDCharacter->HasAuthority())
		{
			FDCharacter->ForceNetUpdate();
		}
	}

	ExecuteComboGameplayCue();
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

void UFDGA_Attack::ServerRPC_ComboNextSection_Implementation(int32 NextComboIndex)
{
	//TODO 여전히 콤보 2,3 은 동기화 문제 있다.
	//콤보 방식 WaitGameplayEvent  으로 수정할 것..
	
	// 서버의 콤보 카운트를 클라와 강제로 맞춤
	CurrentCombo = NextComboIndex;

	// 서버도 몽타주 섹션을 이동 (HitBox 충돌 판정을 위해 필수)
	MontageJumpToSection(GetSection(CurrentCombo));

	ExecuteComboGameplayCue();

	StartComboTimer();
	HasNextComboInput = false;
}

FName UFDGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	return GetSection(CurrentCombo);
}

FName UFDGA_Attack::GetRandomSection()
{
	CurrentCombo = FMath::RandRange(1, CurrentComboData->MaxComboCount);
	return GetSection(CurrentCombo);
}

FName UFDGA_Attack::GetSection(uint8 Index)
{
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, Index);
	return NextSection;
}

void UFDGA_Attack::StartComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	//TODO
	const float ComboEffectiveTime = (CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UFDGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UFDGA_Attack::CheckComboInput()
{
	ComboTimerHandle.Invalidate();
	FString NetMode = GetOwningActorFromActorInfo()->HasAuthority() ? TEXT("Server") : TEXT("Client"); 
	if (HasNextComboInput)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Combo Success! Count: %d"), *NetMode, CurrentCombo + 1);
		MontageJumpToSection(GetNextSection());

		if (!GetOwningActorFromActorInfo()->HasAuthority())
		{
			// 클라이언트(Client): "나 성공했어! 서버야 너도 실행해!" (RPC 전송)
			ServerRPC_ComboNextSection(CurrentCombo);
		}
		else
		{
			// 서버(Server): (만약 운 좋게 타이밍이 맞았다면) GC 발사
			ExecuteComboGameplayCue();
		}

		StartComboTimer();
		HasNextComboInput = false;
	}
	else
	{
		// [실패 로그] 만약 서버에서 이게 찍히면, 입력이 늦게 와서 씹힌 것임
		UE_LOG(LogTemp, Error, TEXT("[%s] Combo Failed! Input False"), *NetMode);
	}
}

void UFDGA_Attack::ExecuteComboGameplayCue()
{
	// 반드시 서버(Authority)에서만 실행해야 함 (NetMulticast로 전파됨)
	if (GetOwningActorFromActorInfo()->HasAuthority())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetOwningActorFromActorInfo()->GetActorLocation();

		// [수정] 태그 이름에 숫자를 붙이는 게 아니라,
		// 파라미터(RawMagnitude)에 콤보 숫자를 담습니다!
		CueParams.RawMagnitude = (float)CurrentCombo;

		FGameplayTag CueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Attack.Combo"));

		if (CueTag.IsValid())
		{
			GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(CueTag, CueParams);
		}
	}
}
