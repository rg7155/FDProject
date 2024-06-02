// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/FDCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/FDCharacterHUDInterface.h"
#include "Interface/FDCharacterItemInterface.h"
#include "FDCharacterPlayer.generated.h"


//인자를 배열로 관리하기 위해 구조체 선언
DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UFDItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

/**
 * 
 */
UCLASS()
class FDPROJECT_API AFDCharacterPlayer : public AFDCharacterBase, public IFDCharacterHUDInterface, public IFDCharacterItemInterface
{
	GENERATED_BODY()
	
public:
	AFDCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Character Control Section
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UFDCharacterControlData* CharacterControlData) override;


// Camera Section
protected:
	void UpdateZoom();
	void ZoomIn();
	void ZoomOut();

	UPROPERTY(EditDefaultsOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float TargetArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float ZoomStep = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float MinZoom = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float MaxZoom = 400.f;

	FTimerHandle ZoomTimerHandle;

	// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShieldAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomInAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomOutAction;

	//void SetInputActionByObjectFinder(TObjectPtr<class UInputAction>& action, const TCHAR* ref);

	virtual void Jump() override;
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void QuaterMove(const FInputActionValue& Value);
	void Attack();
	void Interaction();
	void Shield();
	

	ECharacterControlType CurrentCharacterControlType;
	bool isShopVisible = false;

	// UI Section
protected:
	virtual void SetupHUDWidget(class UFDHUDWidget* InHUDWidget) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UFDHUDWidget> HUDWidget;

	// Item Section
protected:
	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UFDItemData* InItemData) override;
	virtual void DrinkPotion(class UFDItemData* InItemData);
	virtual void EquipWeapon(class UFDItemData* InItemData);
	virtual void ReadScroll(class UFDItemData* InItemData);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ShieldMontage;
	bool isShield = false;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void CameraShake() override;
};
