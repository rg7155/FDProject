// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FDCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOutOfHealthDelegate, AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDataChangedDelegate);

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UFDCharacterAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//바꾸기 전 Value 인자값을 변경할 수 있는
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	//바꾸고 난 후 로그용 
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
public:
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	//이펙트로부터 어트리뷰트 변경될 때
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;


	mutable FOutOfHealthDelegate OnOutOfHealth;
	mutable FDataChangedDelegate OnChanged;
protected:
	void SetClampData(const FGameplayAttribute& Attribute, float& NewValue) const;
	void CreateDamageFont(const AActor* TargetActor);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_MaxMovementSpeed)
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MaxMovementSpeed);

	// --- Attack Info (Everyone - 애니메이션 동기화 위해) ---
	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackRange)
	FGameplayAttributeData AttackRange;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, AttackRange);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_MaxAttackRange)
	FGameplayAttributeData MaxAttackRange;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MaxAttackRange);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackRadius)
	FGameplayAttributeData AttackRadius;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, AttackRadius);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_MaxAttackRadius)
	FGameplayAttributeData MaxAttackRadius;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MaxAttackRadius);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackRate)
	FGameplayAttributeData AttackRate;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, AttackRate);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_MaxAttackRate)
	FGameplayAttributeData MaxAttackRate;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MaxAttackRate);

	UPROPERTY(BlueprintReadOnly, Category = "Gold", ReplicatedUsing = OnRep_Gold)
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, Gold);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, Damage);

	bool bOutOfHealth = false;

public:
	// OnRep Functions
	UFUNCTION() virtual void OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_AttackRange(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_MaxAttackRange(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_AttackRadius(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_MaxAttackRadius(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_AttackRate(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_MaxAttackRate(const FGameplayAttributeData& OldValue);
	UFUNCTION() virtual void OnRep_Gold(const FGameplayAttributeData& OldValue);
};
