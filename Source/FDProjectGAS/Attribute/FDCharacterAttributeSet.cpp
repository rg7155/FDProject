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
	MaxMovementSpeed(600.0f)
{
	InitHealth(GetMaxHealth());
	InitMovementSpeed(GetMaxMovementSpeed());
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
			//태그를 가지고 있다면 false 리턴
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
		OnOutOfHealth.Broadcast();
	}
	bOutOfHealth = (GetHealth() <= 0.0f);
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
			//FDGAS_LOG(LogFDGAS, Log, TEXT("Damage Location : %s"), *SpawnLocation.ToString());//* FString을 const TCHAR*로 변환
		}
	}
	else
	{
		FDGAS_LOG(LogFDGAS, Log, TEXT("ASC has no owner"));
	}
}
