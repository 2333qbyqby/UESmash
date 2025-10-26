// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicCameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Variant_Platforming/PlatformingCharacter.h"

ADynamicCameraManager::ADynamicCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetActorTickEnabled(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	RootComponent = SpringArm;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->SetRelativeRotation(FRotator(-45.0f, -90.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = CameraLagSpeed;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm);
}

void ADynamicCameraManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADynamicCameraManager, TrackedPlayers);
}

void ADynamicCameraManager::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority)
	{
		SetActorTickInterval(0.016f); // ~60 FPS
	}
}

void ADynamicCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() == ROLE_Authority)
	{
		UpdateCameraOnServer();
	}
}

void ADynamicCameraManager::UpdateCameraOnServer()
{
	if (TrackedPlayers.Num() == 0)
	{
		return;
	}

	const FVector Center = CalculatePlayersCenter();
	const float MaxDistance = CalculateMaxPlayerDistance();

	const float NewDistance = FMath::Clamp(MaxDistance * 1.2f, MinCameraDistance, MaxCameraDistance);
	const float LerpedDistance = FMath::FInterpTo(SpringArm->TargetArmLength, NewDistance, GetWorld()->GetDeltaSeconds(), CameraLagSpeed);

	SetActorLocation(Center);
	SpringArm->TargetArmLength = LerpedDistance;
}

FVector ADynamicCameraManager::CalculatePlayersCenter() const
{
	FVector Center = FVector::ZeroVector;
	int32 ValidPlayers = 0;
	for (const auto& Player : TrackedPlayers)
	{
		if (IsValid(Player))
		{
			Center += Player->GetActorLocation();
			ValidPlayers++;
		}
	}
	return ValidPlayers > 0 ? Center / ValidPlayers : GetActorLocation();
}

float ADynamicCameraManager::CalculateMaxPlayerDistance() const
{
	if (TrackedPlayers.Num() < 2)
	{
		return MinCameraDistance;
	}

	FVector Center = CalculatePlayersCenter();
	float MaxDistSq = 0.0f;

	for (const auto& Player : TrackedPlayers)
	{
		if (IsValid(Player))
		{
			MaxDistSq = FMath::Max(MaxDistSq, FVector::DistSquared(Center, Player->GetActorLocation()));
		}
	}
	return FMath::Sqrt(MaxDistSq);
}

void ADynamicCameraManager::OnRep_TrackedPlayers() const
{
	// Optional: Add client-side logic here if needed when the player list updates.
	// For example, updating a UI element that shows tracked players.
	UE_LOG(LogTemp, Log, TEXT("[DynamicCameraManager][Client] TrackedPlayers list replicated. Count: %d"), TrackedPlayers.Num());
}

void ADynamicCameraManager::SetAsViewTargetForAllPlayers()
{
	if (GetWorld())
	{
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				PC->SetViewTargetWithBlend(this, 0.5f);
			}
		}
	}
}

void ADynamicCameraManager::RegisterPlayer(APlatformingCharacter* Player)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Server_RegisterPlayer_Implementation(Player);
	}
	else
	{
		Server_RegisterPlayer(Player);
	}
}

void ADynamicCameraManager::UnregisterPlayer(APlatformingCharacter* Player)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Server_UnregisterPlayer_Implementation(Player);
	}
	else
	{
		Server_UnregisterPlayer(Player);
	}
}

void ADynamicCameraManager::Server_RegisterPlayer_Implementation(APlatformingCharacter* Player)
{
	if (IsValid(Player) && !TrackedPlayers.Contains(Player))
	{
		TrackedPlayers.Add(Player);
		AddTickPrerequisiteActor(Player);
		UE_LOG(LogTemp, Log, TEXT("[DynamicCameraManager][Server] Registered player: %s. Total tracked: %d"), *Player->GetName(), TrackedPlayers.Num());
		
		if (TrackedPlayers.Num() >= 1)
		{
			// When the first player joins, set the view target for everyone.
			Multicast_SetAsViewTarget();
		}
	}
}

void ADynamicCameraManager::Server_UnregisterPlayer_Implementation(APlatformingCharacter* Player)
{
	if (IsValid(Player) && TrackedPlayers.Contains(Player))
	{
		TrackedPlayers.Remove(Player);
		RemoveTickPrerequisiteActor(Player);
		UE_LOG(LogTemp, Log, TEXT("[DynamicCameraManager][Server] Unregistered player: %s. Total tracked: %d"), *Player->GetName(), TrackedPlayers.Num());
	}
}

void ADynamicCameraManager::Multicast_SetAsViewTarget_Implementation()
{
	SetAsViewTargetForAllPlayers();
}
