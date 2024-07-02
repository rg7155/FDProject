// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/FDCharacterAttributeSet.h"

AFDGASCharacterNonPlayer::AFDGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UFDCharacterAttributeSet>(TEXT("AttributeSet"));

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

	//���ؽ�Ʈ �ڵ� ����
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	//���� �ڵ� ����(���ؽ�Ʈ �ڵ鵵 ����)
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		//GA ���� ������ GE�� �ߵ����� ���� ����
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}


