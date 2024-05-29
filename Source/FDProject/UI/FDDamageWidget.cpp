// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDDamageWidget.h"
#include "Components/TextBlock.h"

UFDDamageWidget::UFDDamageWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UFDDamageWidget::NativeConstruct()
{
	DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtDamage")));
	ensure(DamageText);

	DamageText->SetText(FText::AsNumber(FMath::FloorToInt(Damage)));

	//UWidgetAnimation* Anim = AnimationCallbacks[0].Animation;
	//PlayAnimation(Anim);
}

void UFDDamageWidget::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	OwningActor->Destroy();
}

void UFDDamageWidget::SetDamageText(float _Damage)
{
	//이때 DamageText 변수 할당 전임
	Damage = _Damage;
}
