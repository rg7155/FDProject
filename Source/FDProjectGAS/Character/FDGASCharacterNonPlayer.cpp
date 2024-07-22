// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
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
	//FGameplayAbilitySpec* AttackSpec = ASC->FindAbilitySpecFromClass();
	//ASC->TryActivateAbility(Spec->Handle);
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

