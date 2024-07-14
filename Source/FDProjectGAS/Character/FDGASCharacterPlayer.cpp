// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/FDGASPlayerState.h"
#include "EnhancedInputComponent.h"
#include "Character/FDCharacterStatComponent.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "UI/FDHUDWidget.h"

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

void AFDGASCharacterPlayer::SetupGASHUDWidget(UFDGASHUDWidget* InHUDWidget)
{

	GASHUDWidget = InHUDWidget;
	if (GASHUDWidget)
	{
		//GASHUDWidget->UpdateHpBar(Stat->GetCurrentHp());
		//ASC->GetGameplayAttributeValueChangeDelegate(UFDCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UFDGASHpBarUserWidget::OnHealthChanged);
		//ASC->GetGameplayAttributeValueChangeDelegate(UFDCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UFDGASHpBarUserWidget::OnMaxHealthChanged);


		////초기 정보 보여주기 위해 업데이트
		//InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		//InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());
		//InHUDWidget->UpdateGold(Stat->GetGold());

		////스텟에 있는 델리게이트에 위젯의 함수 연동
		//Stat->OnStatChanged.AddUObject(InHUDWidget, &UFDHUDWidget::UpdateStat);
		//Stat->OnHpChanged.AddUObject(InHUDWidget, &UFDHUDWidget::UpdateHpBar);
	}
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

