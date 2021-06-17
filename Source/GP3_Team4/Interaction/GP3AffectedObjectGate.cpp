#include "GP3AffectedObjectGate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GP3InteractionPushAndPull.h"

AGP3AffectedObjectGate::AGP3AffectedObjectGate()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;
	
	Gate1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate1"));
	Gate1->SetupAttachment(RootComponent);
	
	Gate2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate2"));
	Gate2->SetupAttachment(RootComponent);
	
	GateCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("GateCollider"));
	GateCollider->SetupAttachment(RootComponent);
}

void AGP3AffectedObjectGate::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	const float Current = DeltaSeconds / OpeningTime;
	Accumulator += !ReverseMovement ? Current : -Current;

	Accumulator = Accumulator < 0.f ? 0.f : Accumulator;
	Accumulator = Accumulator > 1.f ? 1.f : Accumulator;
	
	Gate1->SetRelativeRotation(FQuat::Slerp(ZeroRotation, Gate1Rotation, Accumulator));
	Gate2->SetRelativeRotation(FQuat::Slerp(FullRotation, Gate2Rotation, Accumulator));
	
	if(Accumulator <= 0.0f || Accumulator >= 1.f)
	{
		ReverseMovement = !ReverseMovement;
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

void AGP3AffectedObjectGate::BeginPlay()
{
	Super::BeginPlay();
	
	Gate1Rotation = FRotator(0.f, Gate1RotationInDegreesZ, 0.f).Quaternion();
	Gate2Rotation = FRotator(0.f, Gate2RotationInDegreesZ, 0.f).Quaternion();

	ReverseMovement = IsOpen;
	if(IsOpen)
	{
		Gate1->SetRelativeRotation(Gate1Rotation);
		Gate2->SetRelativeRotation(Gate2Rotation);
		Accumulator = 1.f;
	}
}

void AGP3AffectedObjectGate::GetAffected()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	PlaySound.Broadcast();

	if(ReverseMovement)
	{
		Accumulator = 1.f;
	}
	else
	{
		Accumulator = 0.f;
	}
}

bool AGP3AffectedObjectGate::TestCollision()
{
	if(PrimaryActorTick.IsTickFunctionEnabled())
	{
		return false;
	}
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::BoxOverlapActors(this, GateCollider->GetComponentLocation(), GateCollider->GetComponentRotation().Quaternion() * GateCollider->GetScaledBoxExtent(), ObjectType, nullptr, IgnoreActors, OutActors );

	for (AActor* OutActor : OutActors)
	{
		UGP3InteractionBase* OverlappingObjectComponent = Cast<UGP3InteractionBase>(OutActor->GetComponentByClass(UGP3InteractionBase::StaticClass()));
		if(OverlappingObjectComponent)
		{
			return false;
		}
	}
	return true;
}
