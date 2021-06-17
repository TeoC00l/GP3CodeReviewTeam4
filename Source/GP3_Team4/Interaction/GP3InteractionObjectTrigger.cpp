#include "GP3InteractionObjectTrigger.h"
#include "GP3AffectedObjectGate.h"
#include "Components/StaticMeshComponent.h"
#include "GP3InteractionBlueprint.h"

AGP3InteractionObjectTrigger::AGP3InteractionObjectTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	LeverBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	RootComponent = LeverBase;

	LeverHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle"));
	LeverHandle->SetupAttachment(RootComponent);

	InteractionComponent = CreateDefaultSubobject<UGP3InteractionBlueprint>(TEXT("InteractionComponent"));
	
}

void AGP3InteractionObjectTrigger::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const float Current = DeltaSeconds / PullTime;
	Accumulator += !ReverseMovement ? Current : -Current;

	LeverHandle->SetRelativeRotation(FQuat::Slerp(FromRotation, ToRotation, Accumulator));

	if(Accumulator <= 0.f || Accumulator >= 1.f)
	{
		ReverseMovement = !ReverseMovement;
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

void AGP3InteractionObjectTrigger::TriggerGates()
{
	for (AGP3AffectedObjectGate* Gate : ObjectsToAffect)
	{
		if(!Gate->TestCollision())
		{
			PlayErrorSound.Broadcast();
			return;
		}
	}

	for (AGP3AffectedObjectGate* Gate : ObjectsToAffect)
	{
		Gate->GetAffected();
	}

	PlayOpeningSound.Broadcast();

	PrimaryActorTick.SetTickFunctionEnable(true);
}

