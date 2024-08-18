// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FDCharacterSkillAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSkillDataChangedDelegate);

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API UFDCharacterSkillAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UFDCharacterSkillAttributeSet();

	ATTRIBUTE_ACCESSORS(UFDCharacterSkillAttributeSet, SkillRange);
	ATTRIBUTE_ACCESSORS(UFDCharacterSkillAttributeSet, MaxSkillRange);
	ATTRIBUTE_ACCESSORS(UFDCharacterSkillAttributeSet, SkillAttackRate);
	ATTRIBUTE_ACCESSORS(UFDCharacterSkillAttributeSet, MaxSkillAttackRate);
	ATTRIBUTE_ACCESSORS(UFDCharacterSkillAttributeSet, SkillEnergy);
	ATTRIBUTE_ACCESSORS(UFDCharacterSkillAttributeSet, MaxSkillEnergy);
	ATTRIBUTE_ACCESSORS(UFDCharacterSkillAttributeSet, SkillCooldown);
	ATTRIBUTE_ACCESSORS(UFDCharacterSkillAttributeSet, SkillRecovery);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	mutable FSkillDataChangedDelegate OnChanged;

protected:
	void SetClampData(const FGameplayAttribute& Attribute, float& NewValue) const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRange;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillEnergy;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillEnergy;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillCooldown;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillRecovery;
};
