// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/FDPlayerController.h"
#include "FDGASPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECTGAS_API AFDGASPlayerController : public AFDPlayerController
{
	GENERATED_BODY()
public:
	AFDGASPlayerController();
protected:
	virtual void BeginPlay() override;

	//생성할 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UFDGASHUDWidget> FDGASHUDWidgetClass;

	//생성한 위젯을 담을 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UFDGASHUDWidget> FDGASHUDWidget;
};
