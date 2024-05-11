// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/FDCharacterStat.h"
#include "FDCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FFDCharacterStat& /*BaseStat*/, const FFDCharacterStat& /*ModifierStat*/);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FDPROJECT_API UFDCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UFDCharacterStatComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE void SetModifierStat(const FFDCharacterStat& InModifierStat) { ModifierStat = InModifierStat; }
	FORCEINLINE void SetBaseStat(const FFDCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }

	FORCEINLINE const FFDCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FFDCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE FFDCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }

	float ApplyDamage(float InDamage);
protected:
	void SetHp(float NewHp);

	//Transient 디스크에 저장하지 않음
	//VisibleInstanceOnly 배치된 캐릭마다 다른 값 사용가능		
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FFDCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FFDCharacterStat ModifierStat;
};
