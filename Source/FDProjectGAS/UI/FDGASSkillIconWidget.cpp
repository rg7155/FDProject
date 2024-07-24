// Fill out your copyright notice in the Description page of Project Settings.


#include "FDGASSkillIconWidget.h"
#include "AbilitySystemComponent.h"
#include "Tag/FDGameplayTag.h"
#include "Attribute/FDCharacterSkillAttributeSet.h"
#include "Components/ProgressBar.h"

void UFDGASSkillIconWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(FDTAG_CHARACTER_SKILLCOOLDOWN, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UFDGASSkillIconWidget::OnSkillCooldownTagChanged);

		PbCooldown->SetPercent(0);
	}
}

void UFDGASSkillIconWidget::PlayCooldownAnim(float Cooldown)
{
	K2_OnPlayCooldownAnim(Cooldown);
}

void UFDGASSkillIconWidget::OnSkillCooldownTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		const UFDCharacterSkillAttributeSet* SkillAttribute = ASC->GetSet<UFDCharacterSkillAttributeSet>();
		if (SkillAttribute)
		{
			//PbCooldown->SetVisibility(ESlateVisibility::Visible);
			PbCooldown->SetPercent(1);

			float Cooldown = SkillAttribute->GetSkillCooldown();
			K2_OnPlayCooldownAnim(Cooldown);
		}
	}
	else
	{
		PbCooldown->SetPercent(0);
	}
}
