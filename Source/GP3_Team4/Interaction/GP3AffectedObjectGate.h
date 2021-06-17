#pragma once
#include "GameFramework/Actor.h"
#include "GP3AffectedObjectGate.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlaySound);

UCLASS()
class AGP3AffectedObjectGate : public AActor
{
	GENERATED_BODY()

	protected:
	AGP3AffectedObjectGate();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	public:

	void GetAffected();

	bool TestCollision();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Gate1;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Gate2;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* GateCollider;

	UPROPERTY(EditInstanceOnly)
	bool IsOpen = false;

	UPROPERTY(EditInstanceOnly, meta = (ClampMin = "1.0"))
	float OpeningTime = 2.0f;

	UPROPERTY(EditDefaultsOnly)
	TArray<float> MeshesToCache;

	UPROPERTY(EditInstanceOnly)
	float Gate1RotationInDegreesZ = 80.f;

	UPROPERTY(EditInstanceOnly)
	float Gate2RotationInDegreesZ = 80.f;

	UPROPERTY(BlueprintAssignable)
	FPlaySound PlaySound;

private:
	float Accumulator = 0.f;

	bool ReverseMovement = false;

	const FQuat ZeroRotation = FQuat::Identity;

	FQuat Gate1Rotation = FQuat(0.0f, 0.0f, 0.7071f, 0.7071f);

	FQuat Gate2Rotation = FQuat(0.0f, 0.0f, 0.7071f, 0.7071f);

	const FQuat FullRotation = FQuat(0.0f, 0.0f, 1.0f, 0.0f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	
};