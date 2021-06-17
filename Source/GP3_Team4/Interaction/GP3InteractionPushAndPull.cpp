#include "GP3InteractionPushAndPull.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GP3_Team4/Interaction/GP3InteractionPlacementComponent.h"
#include "GP3_Team4/PlayerCharacter/GP3PawnPlayer.h"
#include "GP3AffectedObjectGate.h"
#include "Components/BoxComponent.h"

void UGP3InteractionPushAndPull::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> BoxArray;
	GetOwner()->GetComponents(BoxArray, true);

	if(BoxArray.Num() > 0)
	{
		BoxExtentsVector = BoxArray[0]->GetScaledBoxExtent();
		BoxExtentsVector -= FVector::OneVector;
		Box = BoxArray[0];
	}

	TArray<UGP3InteractionPlacementComponent*> ActorList;
	GetOwner()->GetComponents(ActorList);

	for (UGP3InteractionPlacementComponent* Object : ActorList)
	{
		PlacementPositions.Add(Object);
	}
}

void UGP3InteractionPushAndPull::Interact(AGP3PawnPlayer* PawnPlayer)
{
	PlayerRef = PawnPlayer;
	if(!PlacementAllowed(PawnPlayer, PawnPlayer))
	{
		return;
	}
	PawnPlayer->SetInteractionConnection(this);
	PawnPlayer->PushAndPullLock(true);
	PawnPlayer->SetMovementPartner(this, SpeedMultiplier);
	InteractionStart.Broadcast();
	//GetOwner()->AttachToActor(PawnPlayer, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	
}

void UGP3InteractionPushAndPull::StopInteraction(AGP3PawnPlayer* PawnPlayer)
{
	PawnPlayer->PushAndPullLock(false);
	PawnPlayer->SetPushState(false);
	InteractionStop.Broadcast();
	
}

bool UGP3InteractionPushAndPull::PlacementAllowed(AActor* Player, AGP3PawnPlayer* PawnPlayer)
{
	if(PlacementPositions.Num() == 0)
	{
		return false;
	}
	float ShortestDistance = AcceptableInteractionRange * AcceptableInteractionRange;
	for (USceneComponent* Point: PlacementPositions)
	{
		const float Distance = FVector::DistSquared(Point->GetComponentLocation(), Player->GetActorLocation());
		if(Distance < ShortestDistance)
		{
			ShortestDistance = Distance;
			SelectedPosition = Point;
		}
	}
	if(SelectedPosition == nullptr)
	{
		return false;
	}

	// can probably add the player capsule radius to the check later to get a more accurate reading
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Player);
	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle(this, GetOwner()->GetActorLocation(), SelectedPosition->GetComponentLocation(), TraceType, false,
		IgnoreActors, EDrawDebugTrace::None,Hit, true);

	if(Hit.bBlockingHit)
	{
		return false;
	}
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	Info.ExecutionFunction = FName(TEXT("ReleaseMovementLock"));
	Info.Linkage = 0;
	PawnPlayer->SetMovementLockPush(true);
	FVector MoveToLocation = SelectedPosition->GetComponentLocation();
	MoveToLocation.Z = Player->GetActorLocation().Z;
	FVector OwnerZAltercationVector = GetOwner()->GetActorLocation();
	OwnerZAltercationVector.Z = Player->GetActorLocation().Z;
	FVector RotationDirection = (MoveToLocation - OwnerZAltercationVector).GetSafeNormal();
	FRotator Rotation = UKismetMathLibrary::MakeRotFromX(-RotationDirection);
	UKismetSystemLibrary::MoveComponentTo(Player->GetRootComponent(), MoveToLocation, Rotation, false, false,
		1.f, true, EMoveComponentAction::Move, Info);

	PawnPlayer->SetPlayerRotation(Rotation, false);
	SelectedPosition = nullptr;
	return true;
}

bool UGP3InteractionPushAndPull::AllowDropOff()
{
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());

	UE_LOG(LogTemp,Log,TEXT("Testing dropoff"))
	
	UKismetSystemLibrary::BoxOverlapActors(this, GetOwner()->GetActorLocation(), Box->GetComponentRotation().Quaternion() * BoxExtentsVector,
		ObjectType, AGP3AffectedObjectGate::StaticClass(), IgnoreActors, OverlappingActors );
	
	if(OverlappingActors.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Hit an actor: %s"), *OverlappingActors[0]->GetName())
		return false;
	}

	return true;
}

void UGP3InteractionPushAndPull::ReleaseMovementLock()
{
	PlayerRef->SetMovementLockPush(false);
	PlayerRef->SetZeroVelocity();
	PlayerRef->SetPushState(true);
}
