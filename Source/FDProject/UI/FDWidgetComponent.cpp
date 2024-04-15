// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDWidgetComponent.h"
#include "FDUserWidget.h"

void UFDWidgetComponent::InitWidget()
{
	//Super()���� CreateWidget ȣ���� ��
	//�̶� ������ ���� �ν��Ͻ� ���� �� ���� ()
	Super::InitWidget();

	UFDUserWidget* FDUserWidget = Cast<UFDUserWidget>(GetWidget());
	if (FDUserWidget)
	{
		FDUserWidget->SetOwningActor(GetOwner());
	}
}