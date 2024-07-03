// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/FDCharacterAttributeSet.h"
#include "FDProjectGAS.h"
#include "GameplayEffectExtension.h"
#include "Tag/FDGameplayTag.h"	
#include "Actor/FDDamage.h"

UFDCharacterAttributeSet::UFDCharacterAttributeSet() :
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.f),
	MaxAttackRadius(150.0f),
	AttackRate(30.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f),
	Damage(0.0f)
{
	InitHealth(GetMaxHealth());
}

void UFDCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//if (Attribute == GetHealthAttribute())
	//{
	//	NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	//}
	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

void UFDCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		//ABGAS_LOG(LogABGAS, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		FDGAS_LOG(LogFDGAS, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));

		CreateDamageFont(Data.Target.GetOwner());

		SetDamage(0.0f);
	}

	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(FDTAG_CHARACTER_ISDEAD);
		OnOutOfHealth.Broadcast();
	}
	bOutOfHealth = (GetHealth() <= 0.0f);
}

void UFDCharacterAttributeSet::CreateDamageFont(const AActor* TargetActor)
{
	//AActor* TargetActor = Data.Target.GetOwner();
	if (TargetActor)
	{
		//TODO 플레이어 위치 이상함

		FVector SpawnLocation = TargetActor->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
		AActor* DamageActor = GetWorld()->SpawnActor(AFDDamage::StaticClass(), &SpawnLocation, &FRotator::ZeroRotator);
		AFDDamage* FDDamageActor = Cast<AFDDamage>(DamageActor);

		if (FDDamageActor)
		{
			FDDamageActor->SetDamage(GetDamage());
		}
	}
	else
	{
		FDGAS_LOG(LogFDGAS, Log, TEXT("ASC has no owner"));
	}
}
