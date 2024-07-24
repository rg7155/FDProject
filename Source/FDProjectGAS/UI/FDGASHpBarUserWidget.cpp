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
	if (ASC)
	{
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
				SetHpBar();
				SetHpBarBack();
				SetHpTxt();
			}
		}
	}
}

void UFDGASHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentBackHealth = CurrentHealth; //prev
	CurrentHealth = ChangeData.NewValue; // curr
	TargetHealth = ChangeData.NewValue;

	const float HpBarUpdateInterval = 0.01f;
	const float HpBarUpdateDuration = 1.f;

	float TotalTicks = HpBarUpdateDuration / HpBarUpdateInterval; //전체 시간/틱 시간
	HealthChangePerTick = (TargetHealth - CurrentBackHealth) / TotalTicks;

	GetWorld()->GetTimerManager().ClearTimer(HpBarUpdateTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(HpBarUpdateTimerHandle, this, &UFDGASHpBarUserWidget::UpdateHpBarBack, HpBarUpdateInterval, true);

	SetHpBar();
	SetHpTxt();
}

void UFDGASHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;

	SetHpBar();
	SetHpTxt();
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
		SetHpBar();
	}
}

void UFDGASHpBarUserWidget::UpdateHpBarBack()
{
	CurrentBackHealth += HealthChangePerTick;

	if ((HealthChangePerTick < 0 && CurrentBackHealth <= TargetHealth) ||
		(HealthChangePerTick > 0 && CurrentBackHealth >= TargetHealth))
	{
		CurrentBackHealth = TargetHealth;
		GetWorld()->GetTimerManager().ClearTimer(HpBarUpdateTimerHandle);
	}

	SetHpBarBack();
	//SetHpTxt();
}

void UFDGASHpBarUserWidget::SetHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}
}

void UFDGASHpBarUserWidget::SetHpBarBack()
{
	if (PbHpBarBack)
	{
		PbHpBarBack->SetPercent(CurrentBackHealth / CurrentMaxHealth);
	}
}

void UFDGASHpBarUserWidget::SetHpTxt()
{
	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}

