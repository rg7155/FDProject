// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FDCharacterControlData.h"
#include "UI/FDHUDWidget.h"
#include "FDCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FDPlayerController.h"
#include "Item/FDPotionItemData.h"
#include "Item/FDScrollItemData.h"
#include "Item/FDWeaponItemData.h"
#include "FDProject.h"
#include "Animation/FDAnimInstance.h"

AFDCharacterPlayer::AFDCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
	// AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/FDProject/Animation/ABP_FDPlayer.ABP_FDPlayer_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/FDProject/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/FDProject/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/FDProject/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/FDProject/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/FDProject/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/FDProject/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInteractionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/FDProject/Input/Actions/IA_Interaction.IA_Interaction'"));
	if (nullptr != InputActionInteractionRef.Object)
	{
		InteractionAction = InputActionInteractionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ShieldActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/FDProject/Input/Actions/IA_Shield.IA_Shield'"));
	if (nullptr != ShieldActionRef.Object)
	{
		ShieldAction = ShieldActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShieldMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/FDProject/Animation/AM_PlayerShield.AM_PlayerShield'"));
	if (ShieldMontageRef.Object)
	{
		ShieldMontage = ShieldMontageRef.Object;
	}
	

	isShopVisible = false;
	isShield = false;

	CurrentCharacterControlType = ECharacterControlType::Shoulder;

	// Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AFDCharacterPlayer::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AFDCharacterPlayer::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AFDCharacterPlayer::ReadScroll)));
}

void AFDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);
}

void AFDCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}

void AFDCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFDCharacterBase::OnCheckAttackAfterMoveable);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AFDCharacterPlayer::ChangeCharacterControl);

	//EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AFDCharacterBase::OnCheckAttackAfterMoveable);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AFDCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AFDCharacterPlayer::ShoulderLook);

	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AFDCharacterPlayer::QuaterMove);

	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AFDCharacterPlayer::Attack);

	EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &AFDCharacterPlayer::Interaction);
	EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Triggered, this, &AFDCharacterPlayer::Shield);

}

void AFDCharacterPlayer::ChangeCharacterControl()
{
	//V키로 컨트롤 변경하는 함수
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AFDCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	//실제 구현부
	UFDCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AFDCharacterPlayer::SetCharacterControlData(const UFDCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

//void AFDCharacterPlayer::SetInputActionByObjectFinder(TObjectPtr<class UInputAction> action, const TCHAR* ref)
//{
//	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRef(ref);
//	if (nullptr != InputActionRef.Object)
//	{
//		action = InputActionRef.Object;
//	}
//}

void AFDCharacterPlayer::Jump()
{
	OnCheckAttackAfterMoveable();

	Super::Jump();
}

void AFDCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	OnCheckAttackAfterMoveable();

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AFDCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AFDCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AFDCharacterPlayer::Attack()
{
	if(!isShield && !isShopVisible && (GetCharacterMovement()->MovementMode ==  EMovementMode::MOVE_Walking || CurrentCombo > 0))
		ProcessComboCommand();

	//UGameplayStatics::OpenLevel(GetWorld(), FName("Stage0"));
}

void AFDCharacterPlayer::Interaction()
{
	AFDPlayerController* PlayerController = Cast<AFDPlayerController>(GetController());
	if (PlayerController)
	{
		isShopVisible = !isShopVisible;
		PlayerController->ToggleMouseCursor(isShopVisible);
		HUDWidget->SetShopVisible(isShopVisible);
	}
}

void AFDCharacterPlayer::Shield()
{
	if (CurrentCombo > 0)
		return;

	isShield = !isShield;
	UFDAnimInstance* AnimInstance = Cast<UFDAnimInstance>(GetMesh()->GetAnimInstance());
	if (isShield)
	{
		AnimInstance->SetShield(true);
		GetCharacterMovement()->MaxWalkSpeed = 0.5f * (Stat->GetBaseStat().MovementSpeed + Stat->GetModifierStat().MovementSpeed);
		AnimInstance->Montage_Play(ShieldMontage, 1.3f);
	}
	else
	{
		AnimInstance->SetShield(false);
		GetCharacterMovement()->MaxWalkSpeed = Stat->GetBaseStat().MovementSpeed + Stat->GetModifierStat().MovementSpeed;
		AnimInstance->Montage_Stop(0.2f, ShieldMontage);
	}
}

void AFDCharacterPlayer::SetupHUDWidget(UFDHUDWidget* InHUDWidget)
{
	HUDWidget = InHUDWidget;
	ensure(HUDWidget);
	if (HUDWidget)
	{
		//초기 정보 보여주기 위해 업데이트
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());
		InHUDWidget->UpdateGold(Stat->GetGold());

		//스텟에 있는 델리게이트에 위젯의 함수 연동
		Stat->OnStatChanged.AddUObject(InHUDWidget, &UFDHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UFDHUDWidget::UpdateHpBar);
	}
}

void AFDCharacterPlayer::TakeItem(UFDItemData* InItemData)
{
	if (InItemData && Stat->GetGold() >= InItemData->Gold)
	{
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
		Stat->SetGold(Stat->GetGold() - InItemData->Gold);
		HUDWidget->UpdateGold(Stat->GetGold());
	}
}
void AFDCharacterPlayer::DrinkPotion(UFDItemData* InItemData)
{
	UFDPotionItemData* PotionItemData = Cast<UFDPotionItemData>(InItemData);
	if (PotionItemData)
	{
		Stat->HealHp(PotionItemData->HealAmount);
	}
}

void AFDCharacterPlayer::EquipWeapon(UFDItemData* InItemData)
{
	UFDWeaponItemData* WeaponItemData = Cast<UFDWeaponItemData>(InItemData);
	if (WeaponItemData)
	{
		//로딩이 안돼있으면
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			//로딩함
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		//Get으로 얻어옴
		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());

		Stat->AddBaseStat(WeaponItemData->BaseStat);
	}
}

void AFDCharacterPlayer::ReadScroll(UFDItemData* InItemData)
{
	UFDScrollItemData* ScrollItemData = Cast<UFDScrollItemData>(InItemData);
	if (ScrollItemData)
	{
		Stat->AddBaseStat(ScrollItemData->BaseStat);
	}
}

float AFDCharacterPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (isShield)
		DamageAmount *= 0.5f;

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageAmount;
}
