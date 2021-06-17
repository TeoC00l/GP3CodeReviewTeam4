#pragma once

#include "Components/PrimitiveComponent.h"
#include "GP3PortalPiece.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS(Meta = (BlueprintSpawnableComponent))
class UGP3PortalPiece : public UPrimitiveComponent
{
	GENERATED_BODY()
public:

	UGP3PortalPiece();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	UStaticMeshComponent* MeshPiece;

	UPROPERTY(EditAnywhere, Category = Custom)
	UBoxComponent* Box;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	void MoveInDirection(FVector MovementDirection);
	void FinishMovement();
	void Reset();
	void ApplyForce();

	UFUNCTION(BlueprintCallable)
	void SetActiveState(bool SetActive);

	bool IsUsable = true;

private:

	UPROPERTY(EditAnywhere)
	float InputSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float InputDrag = 0.8f;

	UPROPERTY(EditAnywhere)
	float MovementDrag = 0.8f;

	UPROPERTY(EditAnywhere)
	float Bouncyness = 5.f;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> TraceType;

	UPROPERTY(EditAnywhere)
	bool ReverseBounce = false;
	
	float MaxDistance;
	FVector StartingLocation;
	FVector Velocity;
	FVector ForceApplication = FVector::UpVector;
	FVector Movement;
	FVector HalfExtents;
};