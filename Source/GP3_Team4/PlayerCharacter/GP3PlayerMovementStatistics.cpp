#include "GP3PlayerMovementStatistics.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"

FGP3FrameMovement::FGP3FrameMovement(AActor* InActor)
{
	StartLocation = InActor->GetActorLocation();
}

FGP3FrameMovement::FGP3FrameMovement(USceneComponent* InSceneComponent)
{
	StartLocation = InSceneComponent->GetComponentLocation();
}

void FGP3FrameMovement::AddDelta(const FVector& InDelta)
{
	MovementDelta += InDelta;
}
