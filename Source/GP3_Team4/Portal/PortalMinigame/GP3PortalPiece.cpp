#include "GP3PortalPiece.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"

UGP3PortalPiece::UGP3PortalPiece()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UGP3PortalPiece::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	Velocity += (ForceApplication - Velocity * MovementDrag) * DeltaTime;
	Movement += (- Movement * InputSpeed * InputDrag) * DeltaTime;

	TArray<FHitResult> Hit;
	TArray<AActor*> IgnoreActors;
	const FVector SkinWidthVector = (Velocity + Movement).GetSafeNormal() * 1.f;
	const FVector CastDirection = GetOwner()->GetActorRotation().RotateVector((Velocity + Movement + SkinWidthVector) * DeltaTime);
	
	UKismetSystemLibrary::BoxTraceMulti(this, GetComponentLocation(), GetComponentLocation() + CastDirection,
		HalfExtents, GetOwner()->GetActorRotation() , TraceType, false, IgnoreActors, EDrawDebugTrace::None, Hit, false);

	bool CanMove = true;
	for (FHitResult HitResult : Hit)
	{
		if(HitResult.bBlockingHit)
		{
			if(HitResult.Component != MeshPiece)
			{
				CanMove = false;
			}
		}
	}

	if(CanMove)
	{
		AddRelativeLocation((Velocity + Movement) * DeltaTime);
	}

	if(Velocity.Size() > Bouncyness)
	{
		ApplyForce();
	}
}

void UGP3PortalPiece::BeginPlay()
{
	Super::BeginPlay();
	StartingLocation = GetComponentLocation();
	TArray<USceneComponent*> Children = GetAttachChildren();
	ForceApplication *= Bouncyness;
	if(ReverseBounce)
	{
		ApplyForce();
	}

	if(Children.Num() <= 0)
	{
		return;
	}

	for (USceneComponent* Child : Children)
	{
		UStaticMeshComponent* CastTest = Cast<UStaticMeshComponent>(Child);
		if(CastTest)
		{
			MeshPiece = CastTest;
		}

		UBoxComponent* BoxTest = Cast<UBoxComponent>(Child);
		if(BoxTest)
		{
			Box = BoxTest;
			HalfExtents = Box->GetScaledBoxExtent();
		}
	}
	
}

void UGP3PortalPiece::MoveInDirection(FVector MovementDirection)
{
	Movement.X = 0.f;
	Movement.Y += MovementDirection.X;
	Movement.Z += MovementDirection.Y;
}

void UGP3PortalPiece::FinishMovement()
{
	IsUsable = false;
	Movement = FVector::ZeroVector;
	SetActiveState(false);
	if(MeshPiece)
	{
		MeshPiece->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshPiece->SetVisibility(false, false);
	}
}

void UGP3PortalPiece::Reset()
{
	MeshPiece->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshPiece->SetVisibility(true, false);
	SetWorldLocation(StartingLocation);
	SetActiveState(true);
	IsUsable = true;
}

void UGP3PortalPiece::ApplyForce()
{
	ForceApplication *= -1;
}

void UGP3PortalPiece::SetActiveState(bool SetActive)
{
	SetComponentTickEnabled(SetActive);
}

