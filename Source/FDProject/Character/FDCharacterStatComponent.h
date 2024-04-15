// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FDCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

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

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);

protected:
	void SetHp(float NewHp);

	//VisibleInstanceOnly ��ġ�� ĳ������ �ٸ� �� ��밡��		
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	//Transient ��ũ�� �������� ����
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

};
