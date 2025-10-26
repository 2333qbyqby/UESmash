// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DynamicCameraManager.h"
// 前置声明，避免不必要的头文件依赖
class APlatformingCharacter;

#include "PlatformingGameMode.generated.h"

/**
 *  Simple GameMode for a third person platforming game
 */
UCLASS()
class APlatformingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	/** Constructor */
	APlatformingGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<ADynamicCameraManager> CameraManagerClass;
	void SpawnCameraManager();
};