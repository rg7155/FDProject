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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UFDCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MovementSpeed);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, MaxMovementSpeed);
	ATTRIBUTE_ACCESSORS(UFDCharacterAttributeSet, Gold);

	//바꾸기 전 Value 인자값을 변경할 수 있는
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//바꾸고 난 후 로그용 
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
public:
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	//이펙트로부터 어트리뷰트 변경될 때
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	mutable FOutOfHealthDelegate OnOutOfHealth;

protected:
	void CreateDamageFont(const AActor* TargetActor);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MovementSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMovementSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Gold;

	bool bOutOfHealth = false;
};
