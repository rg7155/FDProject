// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDWidgetComponent.h"
#include "FDUserWidget.h"

void UFDWidgetComponent::InitWidget()
{
	//Super()에서 CreateWidget 호출할 때
	//이때 위젯에 대한 인스턴스 생성 된 직후 ()
	Super::InitWidget();

	UFDUserWidget* FDUserWidget = Cast<UFDUserWidget>(GetWidget());
	if (FDUserWidget)
	{
		FDUserWidget->SetOwningActor(GetOwner());
	}
}