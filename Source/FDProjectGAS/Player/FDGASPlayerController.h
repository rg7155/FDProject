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

	//������ ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UFDGASHUDWidget> FDGASHUDWidgetClass;

	//������ ������ ���� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UFDGASHUDWidget> FDGASHUDWidget;
};
