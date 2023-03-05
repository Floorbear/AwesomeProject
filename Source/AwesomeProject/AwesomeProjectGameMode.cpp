// Copyright Epic Games, Inc. All Rights Reserved.

#include "AwesomeProjectGameMode.h"
#include "AwesomeProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAwesomeProjectGameMode::AAwesomeProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
