#pragma once
#include "GameFramework/Actor.h"
#include "GP3InteractionObjectTrigger.generated.h"

class AGP3AffectedObjectGate;
class UStaticMeshComponent;
class UGP3InteractionBlueprint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayOpeningSound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayErrorSound);

UCLASS()
class AGP3InteractionObjectTrigger : public AActor
{
	GENERATED_BODY()

public:

	AGP3InteractionObjectTrigger();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void TriggerGates();

	UPROPERTY(EditAnywhere)
	TArray<AGP3AffectedObjectGate*> ObjectsToAffect;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* LeverBase;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* LeverHandle;

	UPROPERTY()
	UGP3InteractionBlueprint* InteractionComponent;

	UPROPERTY(BlueprintAssignable)
	FPlayOpeningSound PlayOpeningSound;

	UPROPERTY(BlueprintAssignable)
	FPlayErrorSound PlayErrorSound;

private:
	float Accumulator = 0.f;
	float PullTime = 0.8f;
	bool ReverseMovement = false;

	FQuat FromRotation = FQuat::Identity;
	FQuat ToRotation = FQuat(0.f, -0.7071f, 0.f, 0.7071);
};