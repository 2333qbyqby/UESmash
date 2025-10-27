// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Platforming/PlatformingGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "Variant_Platforming/PlatformingCharacter.h"
#include "Engine/Engine.h"

APlatformingGameMode::APlatformingGameMode()
{
	// stub
}
void APlatformingGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void APlatformingGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority())
	{
		int32 TotalPlayers = 0;
		if (AGameStateBase* GS = GetGameState<AGameStateBase>())
		{
			TotalPlayers = GS->PlayerArray.Num();
		}
		int32 RemoteClients = 0;
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				if (!PC->IsLocalController())
				{
					RemoteClients++;
				}
			}
		}
		UE_LOG(LogTemp, Log, TEXT("[PlatformingGM][Server] PostLogin: %s connected. TotalPlayers=%d, RemoteClients=%d"),
			*GetNameSafe(NewPlayer), TotalPlayers, RemoteClients);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
				FString::Printf(TEXT("Joined: %s | TotalPlayers=%d RemoteClients=%d"), *GetNameSafe(NewPlayer), TotalPlayers, RemoteClients));
		}
	}
}

void APlatformingGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (HasAuthority())
	{
		int32 TotalPlayers = 0;
		if (AGameStateBase* GS = GetGameState<AGameStateBase>())
		{
			TotalPlayers = GS->PlayerArray.Num();
		}
		int32 RemoteClients = 0;
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				if (!PC->IsLocalController())
				{
					RemoteClients++;
				}
			}
		}
		UE_LOG(LogTemp, Log, TEXT("[PlatformingGM][Server] Logout: %s disconnected. TotalPlayers=%d, RemoteClients=%d"),
			*GetNameSafe(Exiting), TotalPlayers, RemoteClients);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
				FString::Printf(TEXT("Left: %s | TotalPlayers=%d RemoteClients=%d"), *GetNameSafe(Exiting), TotalPlayers, RemoteClients));
		}
	}
}
