// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/FDAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UFDAT_JumpAndWaitForLanding::UFDAT_JumpAndWaitForLanding()
{
}

UFDAT_JumpAndWaitForLanding* UFDAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	//�ʱ� ���� ������ �ʿ��ϴٸ� ���ڷ� �޾� ���
	UFDAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UFDAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UFDAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UFDAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();

	SetWaitingOnAvatar();
}

void UFDAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UFDAT_JumpAndWaitForLanding::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

void UFDAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}

