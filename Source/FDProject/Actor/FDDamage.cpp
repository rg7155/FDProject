// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FDDamage.h"
#include "UI/FDWidgetComponent.h"
#include "UI/FDDamageWidget.h"
// Sets default values
AFDDamage::AFDDamage()
{
	// Widget Component 
	DamageWidgetComponent = CreateDefaultSubobject<UFDWidgetComponent>(TEXT("DamageWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> DamageWidgetRef(TEXT("/Game/FDProject/UI/WBP_Damage.WBP_Damage_C"));
	if (DamageWidgetRef.Class)
	{
		DamageWidgetComponent->SetWidgetClass(DamageWidgetRef.Class);
		DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		DamageWidgetComponent->SetDrawSize(FVector2D(150.0f, 15.0f));
		DamageWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//DamageWidget->SetHiddenInGame(true);

	RandomUpOffset = FMath::RandRange(80, 100);
	RandomRightOffset = FMath::RandRange(50, 100);

	PrimaryActorTick.bCanEverTick = true;
}

void AFDDamage::SetDamage(float DamageAmount)
{
	//DamageWidgetComponent->InitWidget();
	UUserWidget* UserWidget = DamageWidgetComponent->GetUserWidgetObject();
	UFDDamageWidget* DamageWidget = Cast<UFDDamageWidget>(UserWidget);
	if (DamageWidget)
	{
		DamageWidget->SetDamageText(DamageAmount);
	}
}

// Called when the game starts or when spawned
void AFDDamage::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFDDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Up 벡터와 Right 벡터의 오프셋을 계산합니다.
	FVector UpOffset = GetActorUpVector() * RandomUpOffset * DeltaTime;
	FVector RightOffset = GetActorRightVector() * RandomRightOffset * DeltaTime;

	// 새로운 위치를 계산하여 Actor를 이동시킵니다.
	FVector NewLocation = GetActorLocation() + UpOffset + RightOffset;
	SetActorLocation(NewLocation);
}


