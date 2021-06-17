#include "GP3PlayerMovementComponent.h"
#include "GP3PlayerMovementStatistics.h"
#include "GameFramework/Actor.h"
#include "GP3_Team4/Interaction/GP3InteractionBase.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UGP3PlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Slerp to rotate character over time
	FacingRotationCurrent = FQuat::Slerp(FacingRotationCurrent.Quaternion(), FacingRotationTarget.Quaternion(), FacingRotationSpeed * DeltaTime).Rotator();

	if (FacingRotationCurrent.Equals(FacingRotationTarget))
	{
		FacingRotationCurrent = FacingRotationTarget;
		SetComponentTickEnabled(false);
	}

	GetOwner()->SetActorRotation(FacingRotationCurrent);
	FacingRotationSpeed = 1.0f;
	
}

void UGP3PlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(GetOwner()->GetRootComponent());
	if(Capsule)
	{
		Radius = Capsule->GetScaledCapsuleRadius();
		HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	}
}

FGP3FrameMovement UGP3PlayerMovementComponent::CreateFrameMovement() const
{
	return FGP3FrameMovement(UpdatedComponent);
}

void UGP3PlayerMovementComponent::Move(FGP3FrameMovement& FrameMovement, UGP3InteractionBase* InteractionItem)
{
	AActor* InteractionActor = nullptr;
	Velocity = FrameMovement.Velocity;
	if(InteractionItem != nullptr)
	{
		InteractionActor = InteractionItem->GetOwner();
		
	}

	if(!InteractionActor)
	{
		MoveWithoutPartner(FrameMovement);
	}
	else
	{
		MoveWithPartner(FrameMovement, InteractionActor);
	}

	
}

void UGP3PlayerMovementComponent::MoveWithoutPartner(FGP3FrameMovement& FrameMovement)
{
	FHitResult Hit;

	// First perform movement with gravity
	if (FrameMovement.ShouldApplyGravity())
	{
		SafeMoveUpdatedComponent(FrameMovement.GetGravity(), UpdatedComponent->GetComponentQuat(), true, Hit);
		Hit.Reset();
	}

	FVector Delta = GetMovementDelta(FrameMovement);
	const int MaxIterations = 5;
	int CurrentIterations = 0;

	//This is working code that can be uncommented
	
	if (!Delta.IsNearlyZero())
	{
		SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);

		if (Hit.bBlockingHit)
		{
			Delta = GetMovementDelta(FrameMovement);
		}

		SlideAlongSurface(Delta, 1.0f - Hit.Time, Hit.Normal, Hit);
	}

	/*while(!Delta.IsNearlyZero() && CurrentIterations < MaxIterations)
	{
		
	}*/
	

	//FrameMovement.Hit = Hit;
	//FrameMovement.FinalLocation = UpdatedComponent->GetComponentLocation();
}

/** Makes a combined line trace for the player and the item they are pushing/pulling. No sliding along surfaces are happening while moving in this way. */
void UGP3PlayerMovementComponent::MoveWithPartner(FGP3FrameMovement& FrameMovement, AActor* InteractionPartner)
{
	FVector BoxExtents = (Cast<UBoxComponent>(InteractionPartner->FindComponentByClass(UBoxComponent::StaticClass())))->GetScaledBoxExtent();

	FHitResult PartnerHit;
	FHitResult OwnerHit;

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(InteractionPartner);
	IgnoreActors.Add(GetOwner());
	float ShortestDistance = GetMovementDelta(FrameMovement).Size() + SkinWidth;
	FVector SkinWidthAddition = GetMovementDelta(FrameMovement).GetSafeNormal() * ShortestDistance;

	//Add skin width to the initial trace
	UKismetSystemLibrary::BoxTraceSingle(this, InteractionPartner->GetActorLocation(), InteractionPartner->GetActorLocation() + SkinWidthAddition,
		BoxExtents - FVector::UpVector, InteractionPartner->GetActorRotation(), TraceType, false, IgnoreActors,
		EDrawDebugTrace::None,PartnerHit, true);

	UKismetSystemLibrary::CapsuleTraceSingle(this, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() +SkinWidthAddition, Radius, HalfHeight,
		TraceType, false, IgnoreActors, EDrawDebugTrace::None, OwnerHit, true);

	if(PartnerHit.bBlockingHit)
	{
		ShortestDistance = FMath::Min(ShortestDistance, PartnerHit.Distance);
	}
	
	if(OwnerHit.bBlockingHit)
	{
		ShortestDistance = FMath::Min(ShortestDistance, OwnerHit.Distance);
	}

	ShortestDistance -= SkinWidth;

	if(ShortestDistance < SkinWidth)
	{
		return;
	}
		
	InteractionPartner->SetActorLocation(InteractionPartner->GetActorLocation() + FrameMovement.GetMovementDelta().GetSafeNormal() * ShortestDistance);
	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + FrameMovement.GetMovementDelta().GetSafeNormal() * ShortestDistance);
	
}


void UGP3PlayerMovementComponent::SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed)
{
	Internal_SetFacingRotation(InFacingRotation, InRotationSpeed);
}

void UGP3PlayerMovementComponent::SetFacingRotation(const FQuat& InFacingRotation, float InRotationSpeed)
{
	Internal_SetFacingRotation(InFacingRotation.Rotator(), InRotationSpeed);
}

void UGP3PlayerMovementComponent::SetFacingDirection(const FVector& InFacingDirection, float InRotationSpeed)
{
	Internal_SetFacingRotation(InFacingDirection.Rotation(), InRotationSpeed);
}

void UGP3PlayerMovementComponent::SetForcedRotation(const FRotator& NewRotation)
{
	SetComponentTickEnabled(false);
	FacingRotationCurrent = NewRotation;
	FacingRotationTarget = NewRotation;
	GetOwner()->SetActorRotation(NewRotation);
	
}

void UGP3PlayerMovementComponent::Internal_SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed)
{
	FRotator NewRotation = InFacingRotation;
	NewRotation.Roll = 0.0f;
	NewRotation.Pitch = 0.0f;
	FacingRotationTarget = NewRotation;

	if (InRotationSpeed < 0.0f)
	{
		FacingRotationCurrent = NewRotation;
		SetComponentTickEnabled(false);
	}
	else
	{
		SetComponentTickEnabled(true);
		FacingRotationSpeed = InRotationSpeed;
	}
}

FVector UGP3PlayerMovementComponent::GetMovementDelta(const FGP3FrameMovement& FrameMovement) const
{
	return FrameMovement.GetMovementDelta();
}
