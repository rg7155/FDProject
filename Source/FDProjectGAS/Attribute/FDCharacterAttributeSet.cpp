// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/FDCharacterAttributeSet.h"
#include "FDProjectGAS.h"
#include "GameplayEffectExtension.h"
#include "Tag/FDGameplayTag.h"	
#include "Actor/FDDamage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h" // 필수

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

void UFDCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 모두에게 전송 (체력, 속도, 공격 범위 등)
	DOREPLIFETIME(UFDCharacterAttributeSet, Health);
	DOREPLIFETIME(UFDCharacterAttributeSet, MaxHealth);
	DOREPLIFETIME(UFDCharacterAttributeSet, MovementSpeed);
	DOREPLIFETIME(UFDCharacterAttributeSet, MaxMovementSpeed);
	DOREPLIFETIME(UFDCharacterAttributeSet, AttackRange);
	DOREPLIFETIME(UFDCharacterAttributeSet, MaxAttackRange);
	DOREPLIFETIME(UFDCharacterAttributeSet, AttackRadius);
	DOREPLIFETIME(UFDCharacterAttributeSet, MaxAttackRadius);
	DOREPLIFETIME(UFDCharacterAttributeSet, AttackRate);
	DOREPLIFETIME(UFDCharacterAttributeSet, MaxAttackRate);

	// 주인에게만 전송 (골드)
	DOREPLIFETIME_CONDITION(UFDCharacterAttributeSet, Gold, COND_OwnerOnly);
}

void UFDCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)				{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, Health, OldValue); }
void UFDCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)			{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, MaxHealth, OldValue); }
void UFDCharacterAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldValue)		{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, MovementSpeed, OldValue); }
void UFDCharacterAttributeSet::OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldValue)	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, MaxMovementSpeed, OldValue); }
void UFDCharacterAttributeSet::OnRep_AttackRange(const FGameplayAttributeData& OldValue)		{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, AttackRange, OldValue); }
void UFDCharacterAttributeSet::OnRep_MaxAttackRange(const FGameplayAttributeData& OldValue)		{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, MaxAttackRange, OldValue); }
void UFDCharacterAttributeSet::OnRep_AttackRadius(const FGameplayAttributeData& OldValue)		{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, AttackRadius, OldValue); }
void UFDCharacterAttributeSet::OnRep_MaxAttackRadius(const FGameplayAttributeData& OldValue)	{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, MaxAttackRadius, OldValue); }
void UFDCharacterAttributeSet::OnRep_AttackRate(const FGameplayAttributeData& OldValue)			{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, AttackRate, OldValue); }
void UFDCharacterAttributeSet::OnRep_MaxAttackRate(const FGameplayAttributeData& OldValue)		{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, MaxAttackRate, OldValue); }
void UFDCharacterAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldValue)				{ GAMEPLAYATTRIBUTE_REPNOTIFY(UFDCharacterAttributeSet, Gold, OldValue); }

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

		// [참고] 현재는 서버에서만 폰트 생성됨 (클라에도 띄우려면 GameplayCue 필요)
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
			//FDGAS_LOG(LogFDGAS, Log, TEXT("Damage Location : %s"), *SpawnLocation.ToString());//* FString을 const TCHAR*로 변환
		}
	}
	else
	{
		FDGAS_LOG(LogFDGAS, Log, TEXT("ASC has no owner"));
	}
}
