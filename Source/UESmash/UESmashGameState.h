// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UESmashGameState.generated.h"

/**
 * 
 */
UCLASS()
class UESMASH_API AUESmashGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// 当前侧滚动相机臂长，由游戏模式计算并在网络间复制
	UPROPERTY(Replicated)
	float DynamicSideCameraArmLength = 1600.0f;
};