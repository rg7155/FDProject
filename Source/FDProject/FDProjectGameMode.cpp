// Copyright Epic Games, Inc. All Rights Reserved.

#include "FDProjectGameMode.h"
#include "FDProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFDProjectGameMode::AFDProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
