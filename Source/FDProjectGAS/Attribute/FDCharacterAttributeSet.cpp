// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/FDCharacterAttributeSet.h"
#include "FDProjectGAS.h"
#include "GameplayEffectExtension.h"
#include "Tag/FDGameplayTag.h"	
#include "Actor/FDDamage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UFDCharacterAttributeSet::UFDCharacterAttributeSet() :
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.f),
	MaxAttackRadius(150.0f),
	AttackRate(30.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f),
	Damage(0.0f),
	MovementSpeed(400.0f),
	MaxMovementSpeed(600.0f),
	Gold(1000)
{
	InitHealth(GetMaxHealth());
	InitMovementSpeed(GetMovementSpeed());
}

void UFDCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	SetClampData(Attribute, NewValue);
}

void UFDCharacterAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	SetClampData(Attribute, NewValue);
}

bool UFDCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			if (Data.Target.HasMatchingGameplayTag(FDTAG_CHARACTER_ISSHIELD))
			{
				//FDGAS_LOG(LogFDGAS, Log, TEXT("Damage Magnitude : %f"), Data.EvaluatedData.Magnitude);
				Data.EvaluatedData.Magnitude *= 0.5f;
			}
		}
	}

	return true;
}

void UFDCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMovementSpeedAttribute())
	{
		//SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
		ACharacter* CharacterActor = Cast<ACharacter>(Data.Target.GetAvatarActor());
		if (CharacterActor)
		{
			CharacterActor->GetCharacterMovement()->MaxWalkSpeed = GetMovementSpeed();
		}
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		//FDGAS_LOG(LogFDGAS, Log, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));

		CreateDamageFont(Data.Target.GetAvatarActor());

		SetDamage(0.0f);
	}

	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(FDTAG_CHARACTER_ISDEAD);
		
		AActor* Actor = Data.EffectSpec.GetEffectContext().GetEffectCauser();
		OnOutOfHealth.Broadcast(Actor);
	}
	bOutOfHealth = (GetHealth() <= 0.0f);
}

void UFDCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	OnChanged.Broadcast();
}

void UFDCharacterAttributeSet::SetClampData(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMovementSpeed());
	}
	else if (Attribute == GetAttackRangeAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxAttackRange());
	}
	else if (Attribute == GetAttackRadiusAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxAttackRadius());
	}
	else if (Attribute == GetAttackRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxAttackRate());
	}
	else if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

void UFDCharacterAttributeSet::CreateDamageFont(const AActor* TargetActor)
{
	if (TargetActor)
	{
		FVector SpawnLocation = TargetActor->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
		AActor* DamageActor = GetWorld()->SpawnActor(AFDDamage::StaticClass(), &SpawnLocation, &FRotator::ZeroRotator);
		AFDDamage* FDDamageActor = Cast<AFDDamage>(DamageActor);

		if (FDDamageActor)
		{
			FDDamageActor->SetDamage(GetDamage());
			//FDGAS_LOG(LogFDGAS, Log, TEXT("Damage Location : %s"), *SpawnLocation.ToString());//* FString�� const TCHAR*�� ��ȯ
		}
	}
	else
	{
		FDGAS_LOG(LogFDGAS, Log, TEXT("ASC has no owner"));
	}
}
