// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UFDCharacterStatWidget::NativeConstruct()
{
	//AddToViewport���� ȣ��Ǵ� �Լ�
	Super::NativeConstruct();

	//Stat �� ��� �а� ��Ī�Ǵ� Txt��� ������ ������
	//����ü ���� StaticStruct�� ������
	for (TFieldIterator<FNumericProperty> PropIt(FFDCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UFDCharacterStatWidget::UpdateStat(const FFDCharacterStat& BaseStat, const FFDCharacterStat& ModifierStat)
{
	//FDHUDWidget UpdateStat���� ȣ��Ǵ� �Լ�
	for (TFieldIterator<FNumericProperty> PropIt(FFDCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);
		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);

		//��� �߰�
		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			//SanitizeFloat - float �� ����?
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}
