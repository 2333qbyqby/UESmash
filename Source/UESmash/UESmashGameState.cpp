// Fill out your copyright notice in the Description page of Project Settings.

#include "UESmashGameState.h"
#include "Net/UnrealNetwork.h"

void AUESmashGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AUESmashGameState, DynamicSideCameraArmLength);
}