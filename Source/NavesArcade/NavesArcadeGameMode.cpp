// Copyright Epic Games, Inc. All Rights Reserved.

#include "NavesArcadeGameMode.h"
#include "NaveJugador.h"

ANavesArcadeGameMode::ANavesArcadeGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ANaveJugador::StaticClass();
}
