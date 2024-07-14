// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "FDProjectGAS.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Tag/FDGameplayTag.h"
#include "FDProjectGAS.h"
void UFDGASHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	FDGAS_LOG(LogFDGAS, Log, TEXT("UFDGASHpBarUserWidget 1"));

	if (ASC)
	{
		FDGAS_LOG(LogFDGAS, Log, TEXT("UFDGASHpBarUserWidget 2"));
		ASC->GetGameplayAttributeValueChangeDelegate(UFDCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UFDGASHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UFDCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UFDGASHpBarUserWidget::OnMaxHealthChanged);
		//ASC->RegisterGameplayTagEvent(FDTAG_CHARACTER_INVINSIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UFDGASHpBarUserWidget::OnInvinsibleTagChanged);
		PbHpBar->SetFillColorAndOpacity(HealthColor);

		const UFDCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UFDCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();

			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpBar();
			}
		}
	}
}

void UFDGASHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UFDGASHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UFDGASHpBarUserWidget::OnInvinsibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		PbHpBar->SetFillColorAndOpacity(InvinsibleColor);
		PbHpBar->SetPercent(1.0f);
	}
	else
	{
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateHpBar();
	}
}

void UFDGASHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}

