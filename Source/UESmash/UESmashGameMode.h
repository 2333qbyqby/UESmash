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
	// 头文件中重写声明
protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
