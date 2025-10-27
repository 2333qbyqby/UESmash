// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UESmashGameMode.generated.h"

class AUESmashGameState;

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AUESmashGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AUESmashGameMode();

	// Enable ticking to update dynamic camera target on server
	virtual void Tick(float DeltaSeconds) override;

protected:
	// Whether to drive a dynamic side-scroller camera distance based on players' bounds
	UPROPERTY(EditAnywhere, Category = "Camera|SideScroller")
	bool bEnableDynamicSideCamera = true;

	// Base arm length when players紧密聚集
	UPROPERTY(EditAnywhere, Category = "Camera|SideScroller")
	float DynamicBaseArmLength = 1600.f;

	// Additional zoom scale per world unit of players' spread (width)
	UPROPERTY(EditAnywhere, Category = "Camera|SideScroller")
	float DynamicZoomScalePerUU = 0.8f;

	// Clamp range for the computed arm length
	UPROPERTY(EditAnywhere, Category = "Camera|SideScroller")
	float DynamicMinArmLength = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Camera|SideScroller")
	float DynamicMaxArmLength = 3600.f;

	// Extra margin added to players' width before converting to distance (acts like framing padding)
	UPROPERTY(EditAnywhere, Category = "Camera|SideScroller")
	float DynamicFramingMarginUU = 300.f;

private:
	// Pushes the computed distance to GameState (server authoritative)
	void UpdateDynamicCameraOnServer();
};
