// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASCostBarWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/FDCharacterSkillAttributeSet.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "FDProjectGAS.h"

void UFDGASCostBarWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UFDCharacterSkillAttributeSet::GetSkillEnergyAttribute()).AddUObject(this, &UFDGASCostBarWidget::OnCostChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UFDCharacterSkillAttributeSet::GetMaxSkillEnergyAttribute()).AddUObject(this, &UFDGASCostBarWidget::OnMaxCostChanged);

		const UFDCharacterSkillAttributeSet* CurrentAttributeSet = ASC->GetSet<UFDCharacterSkillAttributeSet>();

		if (CurrentAttributeSet)
		{
			CurrentCost = CurrentAttributeSet->GetSkillEnergy();
			MaxCost = CurrentAttributeSet->GetMaxSkillEnergy();

			SetCostBar();
		}
	}
}
void UFDGASCostBarWidget::OnCostChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentCost = ChangeData.NewValue;
	SetCostBar();

	//TODO UI에서 게임로직 GE를 실행?
	if (CurrentCost < MaxCost)
	{
		if (!SkillCostRecoveryEffectHandle.IsValid())
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(SkillCostRecoveryEffect, 1, EffectContextHandle);
			if (EffectSpecHandle.IsValid())
			{
				SkillCostRecoveryEffectHandle = ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
			}
		}
	}
	else
	{
		bool isRemoved = ASC->RemoveActiveGameplayEffect(SkillCostRecoveryEffectHandle);
		if (isRemoved)
		{
			SkillCostRecoveryEffectHandle.Invalidate();
		}
	}
}

void UFDGASCostBarWidget::OnMaxCostChanged(const FOnAttributeChangeData& ChangeData)
{
	MaxCost = ChangeData.NewValue;
	SetCostBar();
}

void UFDGASCostBarWidget::SetCostBar()
{
	if (PbCostBar)
	{
		PbCostBar->SetPercent(CurrentCost / MaxCost);
	}
}