// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FDCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "FDComboActionData.h"
#include "Physics/FDCollision.h"
#include "Engine/DamageEvents.h"
#include "FDCharacterStatComponent.h"
#include "UI/FDWidgetComponent.h"
#include "UI/FDHpBarWidget.h"
#include "FDProject.h"
#include "Item/FDWeaponItemData.h"

// Sets default values
AFDCharacterBase::AFDCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_FDCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Ragged0.SK_CharM_Ragged0'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/FDProject/Animation/ABP_FDCharacter.ABP_FDCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UFDCharacterControlData> ShoulderDataRef(TEXT("/Script/FDProject.FDCharacterControlData'/Game/FDProject/CharacterControl/FDC_Shoulder.FDC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UFDCharacterControlData> QuaterDataRef(TEXT("/Script/FDProject.FDCharacterControlData'/Game/FDProject/CharacterControl/FDC_Quater.FDC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/FDProject/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// Stat Component 
	Stat = CreateDefaultSubobject<UFDCharacterStatComponent>(TEXT("Stat"));

	// Widget Component 
	HpBar = CreateDefaultSubobject<UFDWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/FDProject/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AFDCharacterBase::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AFDCharacterBase::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AFDCharacterBase::ReadScroll)));


	// Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void AFDCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AFDCharacterBase::SetDead);
}

void AFDCharacterBase::SetCharacterControlData(const UFDCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AFDCharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AFDCharacterBase::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AFDCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AFDCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyComboActionEnd();
}

void AFDCharacterBase::NotifyComboActionEnd()
{
}

void AFDCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AFDCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void AFDCharacterBase::ComboCheck()
{
	//ComboEffectiveTime 이후에 함수 호출됨. 이때 입력이 들어왔었다면
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void AFDCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	//1.엔진 제공 기능 tag 추가 2.복잡한 충돌체 감지 옵션 3.자기 자신만 무시
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = 50.0f;
	const float AttackDamage = Stat->GetTotalStat().Attack;
	//정면에 있는 캡슐의 위치
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	//월드가 제공하는 기능임
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_FDACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		//26:30
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

float AFDCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void AFDCharacterBase::SetDead()
{
	//이동 제한
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
}

void AFDCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f); //DeadMontage는 생성자에서 몽타주 애셋 가져온 거
}

void AFDCharacterBase::SetupCharacterWidget(UFDUserWidget* InUserWidget)
{
	UFDHpBarWidget* HpBarWidget = Cast<UFDHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		//FD_LOG(LogFDProject, Log, TEXT("%s"));

		HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UFDHpBarWidget::UpdateHpBar);
	}
}

void AFDCharacterBase::TakeItem(UFDItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}
void AFDCharacterBase::DrinkPotion(UFDItemData* InItemData)
{
	//UE_LOG(LogABCharacter, Log, TEXT("Drink Potion"));
}

void AFDCharacterBase::EquipWeapon(UFDItemData* InItemData)
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
	}
}

void AFDCharacterBase::ReadScroll(UFDItemData* InItemData)
{
	//UE_LOG(LogABCharacter, Log, TEXT("Read Scroll"));
}

int32 AFDCharacterBase::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void AFDCharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);
}