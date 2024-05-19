// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FDPROJECT_API AFDPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AFDPlayerController();
protected:
	virtual void BeginPlay() override;

public:
	void ToggleMouseCursor(bool isShowCursor);
	// HUD Section
protected:
	//생성할 위젯 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UFDHUDWidget> FDHUDWidgetClass;

	//생성한 위젯을 담을 포인터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UFDHUDWidget> FDHUDWidget;

};
