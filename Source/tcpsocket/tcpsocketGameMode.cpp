// Copyright Epic Games, Inc. All Rights Reserved.

#include "tcpsocketGameMode.h"
#include "tcpsocketCharacter.h"
#include "UObject/ConstructorHelpers.h"

AtcpsocketGameMode::AtcpsocketGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
