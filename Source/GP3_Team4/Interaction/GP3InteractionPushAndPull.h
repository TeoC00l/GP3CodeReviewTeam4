#pragma once

#include "GP3InteractionBase.h"
#include "GP3InteractionPushAndPull.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionStop);

class UGP3InteractionPlacementComponent;
class UBoxComponent;

UCLASS(Meta = (BlueprintSpawnableComponent), DisplayName = "PushAndPullComponent")
class UGP3InteractionPushAndPull : public UGP3InteractionBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
	virtual void Interact(AGP3PawnPlayer* PawnPlayer) override;

	virtual void StopInteraction(AGP3PawnPlayer* PawnPlayer) override;

	bool PlacementAllowed(AActor* Player, AGP3PawnPlayer* PawnPlayer);

	virtual bool AllowDropOff() override;

	UPROPERTY(BlueprintAssignable)
	FInteractionStart InteractionStart;

	UPROPERTY(BlueprintAssignable)
	FInteractionStop InteractionStop;

	UPROPERTY(EditAnywhere)
	TArray<USceneComponent*> PlacementPositions;

	/** The value which will be multiplied into the player's speed when interacting*/
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SpeedMultiplier = 1.f;

	UFUNCTION(BlueprintCallable)
	void ReleaseMovementLock();

private:
	
	TEnumAsByte<ETraceTypeQuery> TraceType;
	
	UPROPERTY(Transient)
	USceneComponent* SelectedPosition;

	UPROPERTY(Transient)
	AGP3PawnPlayer* PlayerRef;

	UPROPERTY(EditAnywhere)
	float AcceptableInteractionRange = 150.f;

	FVector BoxExtentsVector;
	
	UPROPERTY(Transient)
	UBoxComponent* Box;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;

};