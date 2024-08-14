// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/FDGASPlayerState.h"
#include "EnhancedInputComponent.h"
#include "Character/FDCharacterStatComponent.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "UI/FDGASHUDWidget.h"
#include "Item/FDGASItemData.h"
#include "Tag/FDGameplayTag.h"
#include "FDProjectGAS.h"
#include "Game/FDGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"

AFDGASCharacterPlayer::AFDGASCharacterPlayer()
{
	ASC = nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/FDPorjectGAS/Animation/AM_GASPlayerComboAttack.AM_GASPlayerComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKillActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/FDPorjectGAS/Animation/AM_GASPlayerSkillAttack.AM_GASPlayerSkillAttack'"));
	if (SKillActionMontageRef.Object)
	{
		SkillActionMontage = SKillActionMontageRef.Object;
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

		const UFDCharacterAttributeSet* AS = ASC->GetSet<UFDCharacterAttributeSet>();
		if (AS)
		{
			AS->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
		}

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

		GetCharacterMovement()->MaxWalkSpeed = AS->GetMovementSpeed();
	}
}

void AFDGASCharacterPlayer::OnOutOfHealth(AActor* MyInstigator)
{
	SetDead();

	AFDGameMode* FDGameMode = Cast<AFDGameMode>(GetWorld()->GetAuthGameMode());
	if (FDGameMode)
	{
		FDGameMode->OnPlayerDead();
	}
}

void AFDGASCharacterPlayer::Respawn()
{
	Super::Respawn();

	ASC->RemoveLooseGameplayTag(FDTAG_CHARACTER_ISDEAD);
}

void AFDGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupGASInputComponent();
}

void AFDGASCharacterPlayer::Interaction()
{
	Super::Interaction();

	GASHUDWidget->ToggleShopVisible();
}

void AFDGASCharacterPlayer::TakeGASItem(UFDGASItemData* InItemData)
{
	const UFDCharacterAttributeSet* AS = ASC->GetSet<UFDCharacterAttributeSet>();
	if (InItemData && AS->GetGold() >= InItemData->Gold)
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InItemData->ItemEffect, InItemData->Level, EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(FDTAG_DATA_GOLD, -InItemData->Gold);
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);

			//FGameplayEffectSpec* EffectSpec = EffectSpecHandle.Data.Get();
			//if (EffectSpec)
			//{
			//	EffectSpec->SetSetByCallerMagnitude(FDTAG_DATA_GOLD, InItemData->Gold);

			//	ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
			//}
		}
	}
}

void AFDGASCharacterPlayer::SetupGASHUDWidget(UFDGASHUDWidget* InHUDWidget)
{
	GASHUDWidget = InHUDWidget;
}

void AFDGASCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFDGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFDGASCharacterPlayer::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AFDGASCharacterPlayer::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Triggered, this, &AFDGASCharacterPlayer::GASInputPressed, 2);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AFDGASCharacterPlayer::GASInputPressed, 3);
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

