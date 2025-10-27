// Copyright Epic Games, Inc. All Rights Reserved.

#include "UESmashGameMode.h"
#include "UESmashGameState.h"
#include "EngineUtils.h"
#include "UESmashCharacter.h"

AUESmashGameMode::AUESmashGameMode()
{
	// stub
	PrimaryActorTick.bCanEverTick = true;
	// Ensure our custom GameState is used (can still be overridden by BP)
	GameStateClass = AUESmashGameState::StaticClass();
}

