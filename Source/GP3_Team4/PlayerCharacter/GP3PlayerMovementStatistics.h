#pragma once

#include "CoreMinimal.h"

class AActor;
class USceneComponent;

struct FGP3FrameMovement
{
	FGP3FrameMovement(const FVector& InStartLocation) :
        StartLocation(InStartLocation) {}

	FGP3FrameMovement(AActor* InActor);
	FGP3FrameMovement(USceneComponent* InSceneComponent);

	void AddDelta(const FVector& InDelta);
	void AddGravity(const float GravityToApply) { AccumulatedGravity -= GravityToApply; }

	bool ShouldApplyGravity() const { return !FMath::IsNearlyZero(AccumulatedGravity); }

	FVector GetMovementDelta() const { return MovementDelta; }

	FHitResult Hit;

	FVector FinalLocation = FVector::ZeroVector;

	FVector Velocity = FVector::ZeroVector;

	FVector GetStartLocation() const { return StartLocation; }
	FVector GetGravity() const { return FVector(0.0f, 0.0f, AccumulatedGravity); }

private:
	FVector MovementDelta = FVector::ZeroVector;
	FVector StartLocation = FVector::ZeroVector;
	float AccumulatedGravity = 0.0f;
};
