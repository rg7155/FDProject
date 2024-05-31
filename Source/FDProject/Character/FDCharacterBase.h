// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/FDAnimationAttackInterface.h"
#include "Interface/FDCharacterWidgetInterface.h"
#include "GameData/FDCharacterStat.h"
#include "FDCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};


UCLASS()
class FDPROJECT_API AFDCharacterBase : public ACharacter, public IFDAnimationAttackInterface, public IFDCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	AFDCharacterBase();

	virtual void PostInitializeComponents() override;
protected:
	virtual void SetCharacterControlData(const class UFDCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UFDCharacterControlData*> CharacterControlManager;

	// Combo Action Section
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFDComboActionData> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void OnComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void ComboActionEnd();
	virtual void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void OnComboCheck();
	void SetAttackAfterMovementTimer();
	void OnAttackAfterMoveable();
	void OnCheckAttackAfterMoveable();

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;
	FTimerHandle AttackAfterMoveableTimerHandle;
	bool isAttackAfterMoveable = false;

	// Attack Hit Section
protected:
	virtual void AttackHitCheck(float AttackMultiplier) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;
	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFDCharacterStatComponent> Stat;

	// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFDWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UFDUserWidget* InUserWidget) override;

	// Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	// Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);
	void ApplyStat(const FFDCharacterStat& BaseStat, const FFDCharacterStat& ModifierStat);

};
