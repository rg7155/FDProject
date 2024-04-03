// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FDGameMode.h"
#include "FDGameMode.h"
#include "Player/FDPlayerController.h"

AFDGameMode::AFDGameMode()
{
	//static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//if (DefaultPawnClassRef.Class)
	//{
	//	DefaultPawnClass = DefaultPawnClassRef.Class;
	//}

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/FDProject.FDCharacterPlayer"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/FDProject.FDPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
