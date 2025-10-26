// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicCameraManager.generated.h"

class APlatformingCharacter;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class UESMASH_API ADynamicCameraManager : public AActor
{
	GENERATED_BODY()

public:
	ADynamicCameraManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//~ Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	//~ Camera Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float MinCameraDistance = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float MaxCameraDistance = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float CameraLagSpeed = 5.0f;

private:
	//~ Server-side Logic
	void UpdateCameraOnServer();
	FVector CalculatePlayersCenter() const;
	float CalculateMaxPlayerDistance() const;

	//~ Player Tracking (Server-side)
	UPROPERTY(Transient, ReplicatedUsing = OnRep_TrackedPlayers)
	TArray<TObjectPtr<APlatformingCharacter>> TrackedPlayers;

	UFUNCTION()
	void OnRep_TrackedPlayers() const; 

	//~ View Target Logic
	void SetAsViewTargetForAllPlayers();

public:
	//~ Public API for GameMode/Character to call
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void RegisterPlayer(APlatformingCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void UnregisterPlayer(APlatformingCharacter* Player);

	//~ RPCs for registering/unregistering
	UFUNCTION(Server, Reliable)
	void Server_RegisterPlayer(APlatformingCharacter* Player);

	UFUNCTION(Server, Reliable)
	void Server_UnregisterPlayer(APlatformingCharacter* Player);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetAsViewTarget();
};
