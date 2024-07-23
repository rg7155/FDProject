// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilitySpec.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "UI/FDGASWidgetComponent.h"
#include "UI/FDGASUserWidget.h"
#include "FDProjectGAS.h"

AFDGASCharacterNonPlayer::AFDGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UFDCharacterAttributeSet>(TEXT("AttributeSet"));

	HpBar = CreateDefaultSubobject<UFDGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/FDPorjectGAS/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Level = 1;
}

UAbilitySystemComponent* AFDGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFDGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
	AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);

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

	//���ؽ�Ʈ �ڵ� ����(������, ���ؼ���, ��������)
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	//���� �ڵ� ����(������ ���ؽ�Ʈ�� ������)
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	//FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
	if (EffectSpecHandle.IsValid())
	{
		//GA ���� ������ GE�� �ߵ����� ���� ����
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}
void AFDGASCharacterNonPlayer::AttackByAI()
{
	GASInputPressed(0);
}
void AFDGASCharacterNonPlayer::OnOutOfHealth(AActor* MyInstigator)
{
	SetDead();
	if (MyInstigator)
	{
		ApplyGoldEffect(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MyInstigator));
	}
}

void AFDGASCharacterNonPlayer::ApplyGoldEffect(UAbilitySystemComponent* Target)
{
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GoldEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Target);
	}
}


void AFDGASCharacterNonPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			FDGAS_LOG(LogFDGAS, Log, TEXT("IsActive"));
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			FDGAS_LOG(LogFDGAS, Log, TEXT("TryActivate"));
			ASC->TryActivateAbility(Spec->Handle);

			//�̰� �� �����Ƽ ���� �������� ��������Ʈ�� �����??!?!?!?!
			//Spec->Ability->OnGameplayAbilityEnded.AddUObject(this, &AFDGASCharacterNonPlayer::OnAbilityEnded);
		}

	}
}

void AFDGASCharacterNonPlayer::GASInputReleased(int32 InputId)
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

void AFDGASCharacterNonPlayer::OnAbilityEnded(UGameplayAbility* Ability)
{
	int32 AbilityInputID = Ability->GetCurrentAbilitySpec()->InputID;
	//TODO ���� enum �� ��ü
	FDGAS_LOG(LogFDGAS, Log, TEXT("AbilityInputID"));
	if (AbilityInputID == 0)
	{
		FDGAS_LOG(LogFDGAS, Log, TEXT("OnAttackFinished"));
		OnAttackFinished.ExecuteIfBound();
	}
}
