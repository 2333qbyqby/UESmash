// Copyright Epic Games, Inc. All Rights Reserved.

#include "UESmashGameMode.h"
#include "UESmashGameState.h"
#include "GameFramework/PlayerState.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"

AUESmashGameMode::AUESmashGameMode()
{
	// stub
	PrimaryActorTick.bCanEverTick = true;
	// Ensure our custom GameState is used (can still be overridden by BP)
	GameStateClass = AUESmashGameState::StaticClass();
}

void AUESmashGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bEnableDynamicSideCamera)
	{
		return;
	}

	UpdateDynamicCameraOnServer();
}

void AUESmashGameMode::UpdateDynamicCameraOnServer()
{
	// Only server should authoritatively compute
	if (!HasAuthority())
	{
		return;
	}

	// Collect live player characters
	FVector2D MinPos(FLT_MAX, FLT_MAX);
	FVector2D MaxPos(-FLT_MAX, -FLT_MAX);
	int32 NumPlayers = 0;

	for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	{
		APawn* Pawn = *It;
		if (!IsValid(Pawn)) continue;
		if (Pawn->IsActorBeingDestroyed()) continue;
		if (!Pawn->IsPlayerControlled()) continue;
		const FVector Loc = Pawn->GetActorLocation();
		MinPos.X = FMath::Min(MinPos.X, Loc.X);
		MinPos.Y = FMath::Min(MinPos.Y, Loc.Y);
		MaxPos.X = FMath::Max(MaxPos.X, Loc.X);
		MaxPos.Y = FMath::Max(MaxPos.Y, Loc.Y);
		++NumPlayers;
	}

	if (NumPlayers <= 0)
	{
		return; // no players
	}

	// Compute spread in horizontal plane (choose the larger axis to frame)
	const float WidthX = MaxPos.X - MinPos.X;
	const float WidthY = MaxPos.Y - MinPos.Y;
	const float MaxWidth = FMath::Max(WidthX, WidthY);

	// Add margin so players靠近边界时相机仍留白
	const float FramedWidth = MaxWidth + DynamicFramingMarginUU;

	// Convert width to desired arm length (linear mapping)
	float DesiredArm = DynamicBaseArmLength + FramedWidth * DynamicZoomScalePerUU;
	DesiredArm = FMath::Clamp(DesiredArm, DynamicMinArmLength, DynamicMaxArmLength);

	// Write to GameState for replication
	AUESmashGameState* GSM = GetGameState<AUESmashGameState>();
	if (GSM)
	{
		GSM->DynamicSideCameraArmLength = DesiredArm;
	}
}
