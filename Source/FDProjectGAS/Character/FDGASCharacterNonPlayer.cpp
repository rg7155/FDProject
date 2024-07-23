// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FDGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilitySpec.h"
#include "Attribute/FDCharacterAttributeSet.h"
#include "UI/FDGASWidgetComponent.h"
#include "UI/FDGASUserWidget.h"
#include "FDProjectGAS.h"

AFDGASCharacterNonPlayer::AFDGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UFDCharacterAttributeSet>(TEXT("AttributeSet"));

	HpBar = CreateDefaultSubobject<UFDGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/FDPorjectGAS/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Level = 1;
}

UAbilitySystemComponent* AFDGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFDGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);
	AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}

	for (const auto& StartInputAbility : StartInputAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
		StartSpec.InputID = StartInputAbility.Key;
		ASC->GiveAbility(StartSpec);
	}

	//컨텍스트 핸들 생성(가해자, 가해수단, 판정정보)
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	//스펙 핸들 생성(스펙이 컨텍스트를 관리함)
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	//FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
	if (EffectSpecHandle.IsValid())
	{
		//GA 없이 스스로 GE를 발동시켜 스텟 변경
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}
void AFDGASCharacterNonPlayer::AttackByAI()
{
	GASInputPressed(0);
}
void AFDGASCharacterNonPlayer::OnOutOfHealth(AActor* MyInstigator)
{
	SetDead();
	if (MyInstigator)
	{
		ApplyGoldEffect(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MyInstigator));
	}
}

void AFDGASCharacterNonPlayer::ApplyGoldEffect(UAbilitySystemComponent* Target)
{
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GoldEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Target);
	}
}


void AFDGASCharacterNonPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			FDGAS_LOG(LogFDGAS, Log, TEXT("IsActive"));
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			FDGAS_LOG(LogFDGAS, Log, TEXT("TryActivate"));
			ASC->TryActivateAbility(Spec->Handle);

			//이거 왜 어빌리티 종료 시점에는 델리게이트가 사라짐??!?!?!?!
			//Spec->Ability->OnGameplayAbilityEnded.AddUObject(this, &AFDGASCharacterNonPlayer::OnAbilityEnded);
		}

	}
}

void AFDGASCharacterNonPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AFDGASCharacterNonPlayer::OnAbilityEnded(UGameplayAbility* Ability)
{
	int32 AbilityInputID = Ability->GetCurrentAbilitySpec()->InputID;
	//TODO 추후 enum 등 교체
	FDGAS_LOG(LogFDGAS, Log, TEXT("AbilityInputID"));
	if (AbilityInputID == 0)
	{
		FDGAS_LOG(LogFDGAS, Log, TEXT("OnAttackFinished"));
		OnAttackFinished.ExecuteIfBound();
	}
}
