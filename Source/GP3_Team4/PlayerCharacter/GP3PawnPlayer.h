#pragma once

#include "GameFramework/Pawn.h"
#include "GP3PawnPlayer.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class UCameraComponent;
class UGP3PlayerMovementComponent;
class USpringArmComponent;
class UGP3InteractionBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPerformVault, FVector, LedgeNormal, FVector, LedgeTop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSprint, bool, SprintState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrouch, bool, CrouchState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPush, bool, PushState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBecomeAirborne, bool, Airborne);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBlendCameraView, AActor*, BlendTarget, float, Duration);

UCLASS()
class AGP3PawnPlayer : public APawn
{
	GENERATED_BODY()

public:

	AGP3PawnPlayer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Collision)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	//movement component
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UGP3PlayerMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category=Camera)
	float VerticalSensitivity = 45.f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, Category=Camera)
	float HorizontalSensitivity = 45.f;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
	TEnumAsByte<ETraceTypeQuery> VaultTrace;

	UFUNCTION(BlueprintPure)
	float GetHorizontalSensitivity() const;
	
	UFUNCTION(BlueprintPure)
	float GetVerticalSensitivity() const;
	
	UFUNCTION(BlueprintCallable)
	void SetHorizontalSensitivity(float Value);
	
	UFUNCTION(BlueprintCallable)
	void SetVerticalSensitivity(float Value);

	UFUNCTION(BlueprintCallable)
	void BlendCameraView(AActor* BlendTarget, float Duration);

	UPROPERTY(EditAnywhere, Category=Movement)
	float MovingRotationSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category=Movement)
	float StationaryRotationSpeed = 2.f;

	UPROPERTY(BlueprintAssignable)
	FPerformVault PerformVault;
	
	UPROPERTY(BlueprintAssignable)
	FSprint PerformSprint;
	
	UPROPERTY(BlueprintAssignable)
	FCrouch PerformCrouch;
	
	UPROPERTY(BlueprintAssignable)
	FPush PerformPush;

	UPROPERTY(BlueprintAssignable)
	FBecomeAirborne BecomeAirborne;

	UPROPERTY(BlueprintAssignable)
	FBlendCameraView CameraBlend;
	
	void SetInteractionConnection(UGP3InteractionBase* InteractionPartner);
	void SetMovementPartner(UGP3InteractionBase* InteractionPartner, float NewInteractionSpeed);
	
	UFUNCTION(BlueprintCallable)
	void SetMovementLock(bool State);

	virtual void Tick(float DeltaSeconds) override;
	void PushAndPullLock(bool LockState);

	UFUNCTION(BlueprintCallable)
	void SetPlayerRotation(FRotator NewRotation, bool SnapCamera = true);

	void SetCameraPositionAndRotation(FVector NewLocation, FRotator NewRotation);

	void SetCameraPosition(FVector NewLocation);

	UFUNCTION(BlueprintCallable)
	void TeleportToCheckpoint();

	UFUNCTION(BlueprintCallable)
	void SetCameraLock(bool State);

	void SetPushState(bool State);

	void SetMovementLockPush(bool State);

	void SetZeroVelocity();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MovementSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float Gravity = 900.f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintMultiplier = 1.3f;

	UPROPERTY(EditAnywhere, Category = Movement)
	float CrouchMultiplier = 0.6f;

	UPROPERTY(EditAnywhere, Category= Movement)
	float CrouchHalfHeight = 30.f;
	
	FVector InputVector = FVector::ZeroVector;
	FVector LookVector = FVector::ZeroVector;
	FVector RotationDirection;
	float CurrentRotationSpeed;

	/** Handles moving forward/backward */
	void HandleMoveForward(float Val);

	/** Handles strafing movement, left and right */
	void HandleMoveRight(float Val);

	void HandleLookRight(float Rate);
	void HandleLookUp(float Rate);

	void HandleStartInteract();

	void HandleStartCrouch();
	void HandleEndCrouch();

	void HandleStartSprint();
	void HandleEndSprint();

	void HandleVault();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void TestVault();

	bool GroundCheck();

	UCameraComponent* GetCameraComponent() const { return Camera; }

	UCapsuleComponent* GetCapsule() const { return Capsule; }

	TEnumAsByte<ETraceTypeQuery> TraceType;

	UFUNCTION(BlueprintCallable)
	void LerpCameraBoomToTarget(FTransform TargetTrans);

	UFUNCTION(BlueprintCallable)
	void LerpCameraBoomToPlayer();
	
	UFUNCTION()
	void LerpCameraPos(float DeltaTime);

	UFUNCTION(BlueprintPure)
	UCameraComponent* GetCamera() const;

private:
	UPROPERTY()
	float CameraLerpTime = 1;
	float CameraLerpAccumulator = 0;
	bool bFinishedLerp = true;
	bool bLockCameraRotation = true;
	bool bShouldUnlockControl = false;

	FVector PreviousLocationForLerp;
	FVector TargetLocationForLerp;
	FQuat PreviousRotationForLerp;
	FQuat TargetRotationForLerp;
	
	
	bool IsPushAndPullLocked = false;
	float PushAndPullSpeedReduction = 0.5f;

	float SprintModifier = 1.f;
	float CrouchModifier = 1.f;

	bool IsCrouching = false;
	bool IsSprinting = false;
	bool IsMovementLocked = false;

	bool IsAirborne = false;

	float VaultingMaxHeight = 70.f;

	UPROPERTY(Transient)
	UGP3InteractionBase* ConnectedInteractionItem;

	UPROPERTY(Transient)
	UGP3InteractionBase* MovementPartner;

	float CapsuleHalfHeight;
	float CapsuleRadius;

};