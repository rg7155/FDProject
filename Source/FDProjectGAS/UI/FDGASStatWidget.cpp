// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASStatWidget.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "Components/TextBlock.h"
#include "FDProjectGAS.h"

void UFDGASStatWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	if (ASC)
	{
		const UFDCharacterAttributeSet* AS = ASC->GetSet<UFDCharacterAttributeSet>();
		AS->OnChanged.AddDynamic(this, &ThisClass::OnUpdateStat);
	}

	for (TFieldIterator<FProperty> PropIt(UFDCharacterAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		//const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());
		//FDGAS_LOG(LogFDGAS, Log, TEXT("Name : %s"), *TextBaseControlName.ToString());
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());
		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}

	}
	UpdateStat();
}

void UFDGASStatWidget::OnUpdateStat()
{
	UpdateStat();
}

void UFDGASStatWidget::UpdateStat()
{
	const UFDCharacterAttributeSet* AS = ASC->GetSet<UFDCharacterAttributeSet>();

	for (TFieldIterator<FProperty> PropIt(UFDCharacterAttributeSet::StaticClass()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		FGameplayAttributeData BaseData(0.f);

		PropIt->GetValue_InContainer(AS, &BaseData);

		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData.GetBaseValue())));
		}

		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			float ModifierValue = BaseData.GetCurrentValue() - BaseData.GetBaseValue();
			FLinearColor LinearColor = FMath::IsNearlyZero(ModifierValue) ? FLinearColor::White : FLinearColor::Red;
			(*ModifierTextBlockPtr)->SetColorAndOpacity(LinearColor);
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierValue)));
		}
	}
}
