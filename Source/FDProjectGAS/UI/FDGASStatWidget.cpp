// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASStatWidget.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "Attribute/FDCharacterSkillAttributeSet.h"
#include "Components/TextBlock.h"
#include "FDProjectGAS.h"

void UFDGASStatWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	for (TFieldIterator<FProperty> PropIt(UFDCharacterAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		AddLockUp(PropIt->GetName());
	}

	for (TFieldIterator<FProperty> PropIt(UFDCharacterSkillAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		AddLockUp(PropIt->GetName());
	}

	// 기본 스탯 감지 연결
	for (TFieldIterator<FProperty> PropIt(UFDCharacterAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		FGameplayAttribute Attribute(*PropIt);

		if (Attribute.IsValid())
		{
			// 이 속성이 변하면 UpdateStat 함수를 실행
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &ThisClass::OnAnyAttributeChanged);
		}
	}

	// 스킬 스탯 감지 연결
	for (TFieldIterator<FProperty> PropIt(UFDCharacterSkillAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		FGameplayAttribute Attribute(*PropIt);
		if (Attribute.IsValid())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &ThisClass::OnAnyAttributeChanged);
		}
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

void UFDGASStatWidget::OnAnyAttributeChanged(const FOnAttributeChangeData& Data)
{
	UpdateStat();
	UpdateSkillStat();
}

void UFDGASStatWidget::UpdateStat()
{
	const UFDCharacterAttributeSet* AS = ASC->GetSet<UFDCharacterAttributeSet>();
	for (TFieldIterator<FProperty> PropIt(UFDCharacterAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		// 현재 값 (Modifiers 포함)
		FGameplayAttribute Attribute(*PropIt);
		float CurrentVal = ASC->GetNumericAttribute(Attribute);

		// 베이스 값 (순수 값)
		FGameplayAttributeData* AttributeData = PropIt->ContainerPtrToValuePtr<FGameplayAttributeData>(const_cast<UFDCharacterAttributeSet*>(AS));
		float BaseVal = AttributeData ? AttributeData->GetBaseValue() : 0.f;

		UpdateStatText(PropKey, BaseVal, CurrentVal);
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
