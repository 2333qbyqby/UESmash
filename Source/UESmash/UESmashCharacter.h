// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "UESmashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AUESmashCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

public:

	/** Constructor */
	AUESmashCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// --- Side-scroller camera settings (Smash-style) ---

	/** Enable side-scroller camera (when true camera is positioned on the side, facing the arena) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|SideScroller")
	bool bEnableSideScrollerCamera = true;

	/** World yaw for the side camera (default 90 = right side) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|SideScroller")
	float SideCameraYaw = 90.f;

	/** Pitch angle for the side camera (negative looks slightly down) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|SideScroller")
	float SideCameraPitch = -10.f;

	/** Distance from the character to the camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|SideScroller")
	float SideCameraDistance = 800.f;

	/** Optional offset of the camera socket (use to raise/lower camera) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|SideScroller")
	FVector SideCameraSocketOffset = FVector::ZeroVector;
};
