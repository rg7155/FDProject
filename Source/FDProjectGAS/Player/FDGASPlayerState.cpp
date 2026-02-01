// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FDGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "Attribute/FDCharacterSkillAttributeSet.h"

AFDGASPlayerState::AFDGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	// Mixed: 내 폰에는 모든 정보(쿨타임 등) 전송, 남의 폰에는 최소 정보(이펙트/태그)만 전송
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UFDCharacterAttributeSet>(TEXT("AttributeSet"));
	SkillAttributeSet = CreateDefaultSubobject<UFDCharacterSkillAttributeSet>(TEXT("SkillAttributeSet"));
}

UAbilitySystemComponent* AFDGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
