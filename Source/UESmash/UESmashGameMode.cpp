// Copyright Epic Games, Inc. All Rights Reserved.

#include "UESmashGameMode.h"
#include "UESmashGameState.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h" 
AUESmashGameMode::AUESmashGameMode()
{
	// stub
	PrimaryActorTick.bCanEverTick = true;
	// Ensure our custom GameState is used (can still be overridden by BP)
	GameStateClass = AUESmashGameState::StaticClass();
}

AActor* AUESmashGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PlayerStarts;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		PlayerStarts.Add(*It);
	}
 
	int32 PlayerIndex = 0;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (Iterator->Get() == Player)
		{
			break;
		}
		++PlayerIndex;
	}
 
	if (PlayerStarts.Num() > 0)
	{
		int32 SpawnIndex = PlayerIndex % PlayerStarts.Num();
		return PlayerStarts[SpawnIndex];
	}
 
	return Super::ChoosePlayerStart_Implementation(Player);
}