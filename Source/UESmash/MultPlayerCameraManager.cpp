// Fill out your copyright notice in the Description page of Project Settings.


#include "MultPlayerCameraManager.h"

#include "UESmashCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AMultPlayerCameraManager::AMultPlayerCameraManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
 
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = SideCameraDistance;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SocketOffset = SideCameraSocketOffset;
 
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AMultPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
	// 将所有玩家的视角切换到本摄像机管理器
	UWorld* World = GetWorld();
	if (World)
	{
		for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* PC = It->Get();
			if (PC)
			{
				PC->SetViewTarget(this);
			}
		}
	}
}

// Called every frame
void AMultPlayerCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnsureAllPlayersUseThisCamera();
	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUESmashCharacter::StaticClass(), PlayerActors);
	if (PlayerActors.Num() == 0) return;
 
	FVector BoundsCenter, BoundsExtent;
	UGameplayStatics::GetActorArrayBounds(PlayerActors, false, BoundsCenter, BoundsExtent);
	//获取原来的摄像机位置
	FVector OldLocation = CameraBoom->GetComponentLocation();
	FVector NewLocation = FVector(OldLocation.X,BoundsCenter.Y, BoundsCenter.Z );
	CameraBoom->SetWorldLocation(NewLocation);
	CameraBoom->SetWorldRotation(FRotator(SideCameraPitch, SideCameraYaw, 0.f));
	CameraBoom->TargetArmLength = SideCameraDistance;
	CameraBoom->SocketOffset = SideCameraSocketOffset;
}

void AMultPlayerCameraManager::EnsureAllPlayersUseThisCamera()
 {
	UWorld* World = GetWorld();
	if (!World) return;
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetViewTarget() != this)
		{
			PC->SetViewTarget(this);
		}
	}
}