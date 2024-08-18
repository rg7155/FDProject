// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASStatWidget.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "Attribute/FDCharacterSkillAttributeSet.h"
#include "Components/TextBlock.h"
#include "FDProjectGAS.h"

void UFDGASStatWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	if (ASC)
	{
		const UFDCharacterAttributeSet* AS = ASC->GetSet<UFDCharacterAttributeSet>();
		AS->OnChanged.AddDynamic(this, &ThisClass::OnUpdateStat);

		const UFDCharacterSkillAttributeSet* SkillAS = ASC->GetSet<UFDCharacterSkillAttributeSet>();
		SkillAS->OnChanged.AddDynamic(this, &ThisClass::OnUpdateSkillStat);
	}

	for (TFieldIterator<FProperty> PropIt(UFDCharacterAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		AddLockUp(PropIt->GetName());
	}

	for (TFieldIterator<FProperty> PropIt(UFDCharacterSkillAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		AddLockUp(PropIt->GetName());
	}
	UpdateStat();
	UpdateSkillStat();
}

void UFDGASStatWidget::AddLockUp(const FString& PropName)
{
	const FName PropKey(PropName);
	const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropName);
	UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
	if (BaseTextBlock)
	{
		BaseLookup.Add(PropKey, BaseTextBlock);
	}

	const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropName);
	UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
	if (ModifierTextBlock)
	{
		ModifierLookup.Add(PropKey, ModifierTextBlock);
	}
}

void UFDGASStatWidget::OnUpdateStat()
{
	UpdateStat();
}

void UFDGASStatWidget::OnUpdateSkillStat()
{
	UpdateSkillStat();
}

void UFDGASStatWidget::UpdateStat()
{
	const UFDCharacterAttributeSet* AS = ASC->GetSet<UFDCharacterAttributeSet>();
	for (TFieldIterator<FProperty> PropIt(UFDCharacterAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		FGameplayAttributeData BaseData(0.f);

		PropIt->GetValue_InContainer(AS, &BaseData);

		UpdateStatText(PropKey, BaseData.GetBaseValue(), BaseData.GetCurrentValue());
	}
}

void UFDGASStatWidget::UpdateSkillStat()
{
	const UFDCharacterSkillAttributeSet* AS = ASC->GetSet<UFDCharacterSkillAttributeSet>();
	for (TFieldIterator<FProperty> PropIt(UFDCharacterSkillAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		FGameplayAttributeData BaseData(0.f);

		PropIt->GetValue_InContainer(AS, &BaseData);

		UpdateStatText(PropKey, BaseData.GetBaseValue(), BaseData.GetCurrentValue());
	}
}

void UFDGASStatWidget::UpdateStatText(const FName& PropKey, float BaseValue, float CurrentValue)
{
	BaseValue = FMath::RoundToFloat(BaseValue * 10.0f) / 10.0f;
	CurrentValue = FMath::RoundToFloat(CurrentValue * 10.0f) / 10.0f;

	UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
	if (BaseTextBlockPtr)
	{
		(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseValue)));
	}

	UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
	if (ModifierTextBlockPtr)
	{
		float ModifierValue = CurrentValue - BaseValue;
		FLinearColor LinearColor = FMath::IsNearlyZero(ModifierValue) ? FLinearColor::White : FLinearColor::Red;
		(*ModifierTextBlockPtr)->SetColorAndOpacity(LinearColor);
		(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierValue)));
	}
}
