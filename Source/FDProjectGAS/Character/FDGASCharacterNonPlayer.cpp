// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "UI/FDGASWidgetComponent.h"
#include "UI/FDGASUserWidget.h"

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

	//컨텍스트 핸들 생성(가해자, 가해수단, 판정정보)
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	//스펙 핸들 생성(스펙이 컨텍스트를 관리함)
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		//GA 없이 스스로 GE를 발동시켜 스텟 변경
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}
void AFDGASCharacterNonPlayer::OnOutOfHealth()
{
	SetDead();
}

