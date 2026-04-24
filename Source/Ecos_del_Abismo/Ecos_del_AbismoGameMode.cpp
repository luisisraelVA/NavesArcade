// Copyright Epic Games, Inc. All Rights Reserved.

#include "Ecos_del_AbismoGameMode.h"
#include "Ecos_del_AbismoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEcos_del_AbismoGameMode::AEcos_del_AbismoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
