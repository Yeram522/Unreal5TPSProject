// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/TPSProjectGameMode.h"
#include "Character/TPSProjectCharacter.h"
#include "Game/TPSProject.h"
#include "UObject/ConstructorHelpers.h"



ATPSProjectGameMode::ATPSProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PRINT_CALLINFO();
	PRINT_LOG(TEXT("My Log : %s"), TEXT("TPS Project!!"));
}
