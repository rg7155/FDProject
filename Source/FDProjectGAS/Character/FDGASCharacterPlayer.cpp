// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/FDGASPlayerState.h"
#include "EnhancedInputComponent.h"
#include "Character/FDCharacterStatComponent.h"
#include "Attribute/FDCharacterAttributeSet.h"

AFDGASCharacterPlayer::AFDGASCharacterPlayer()
{
	ASC = nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/FDPorjectGAS/Animation/AM_GASPlayerComboAttack.AM_GASPlayerComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}
}

UAbilitySystemComponent* AFDGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFDGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AFDGASPlayerState* GASPS = GetPlayerState<AFDGASPlayerState>();
	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);

		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}

		SetupGASInputComponent();

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));

		//Stat
		const UFDCharacterAttributeSet* AS = ASC->GetSet<UFDCharacterAttributeSet>();
		//Stat->GetBaseStat().
		//Stat->
	}
}

void AFDGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupGASInputComponent();
}

void AFDGASCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFDGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFDGASCharacterPlayer::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AFDGASCharacterPlayer::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AFDGASCharacterPlayer::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Triggered, this, &AFDGASCharacterPlayer::GASInputPressed, 2);
	}
}

void AFDGASCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AFDGASCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}
