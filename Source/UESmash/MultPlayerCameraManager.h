// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MultPlayerCameraManager.generated.h"

class USpringArmComponent;
class UCameraComponent;
UCLASS()
class UESMASH_API AMultPlayerCameraManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMultPlayerCameraManager();
protected:
	virtual void BeginPlay() override;
	// Called when the game starts or when spawned
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;
	// 可调节参数
	UPROPERTY(EditAnywhere, Category = "Camera")
	float SideCameraDistance = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float SideCameraPitch = -20.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float SideCameraYaw = 0.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector SideCameraSocketOffset = FVector::ZeroVector;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void EnsureAllPlayersUseThisCamera();
};
