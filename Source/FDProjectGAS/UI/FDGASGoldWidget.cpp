// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDGASGoldWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "Components/TextBlock.h"

void UFDGASGoldWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		//ASC->GetGameplayAttributeValueChangeDelegate(UFDCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UFDGASHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UFDCharacterAttributeSet::GetGoldAttribute()).AddUObject(this, &UFDGASGoldWidget::OnGoldChanged);

		const UFDCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UFDCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			Gold = CurrentAttributeSet->GetGold();
			UpdateGoldText();
		}
	}
}

void UFDGASGoldWidget::OnGoldChanged(const FOnAttributeChangeData& ChangeData)
{
	//헤더에서 함수 매개변수 헤더 추가해줘야함
	Gold = ChangeData.NewValue;
	UpdateGoldText();
}

void UFDGASGoldWidget::UpdateGoldText()
{
	if (TxtGold)
	{
		TxtGold->SetText(FText::FromString(FString::Printf(TEXT("%d Gold"), Gold)));
	}
}
