#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "GP3PlayerMovementComponent.generated.h"

struct FGP3FrameMovement;
class UGP3InteractionBase;

UCLASS()
class UGP3PlayerMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	FGP3FrameMovement CreateFrameMovement() const;

	void Move(FGP3FrameMovement& FrameMovement, UGP3InteractionBase* InteractionItem = nullptr);

	void MoveWithoutPartner(FGP3FrameMovement& FrameMovement);
	void MoveWithPartner(FGP3FrameMovement& FrameMovement, AActor* InteractionPartner);
	
	FRotator GetFacingRotation() const { return FacingRotationCurrent; }
	FVector GetFacingDirection() const { return FacingRotationCurrent.Vector(); }

	void SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed = -1.0f);
	void SetFacingRotation(const FQuat& InFacingRotation, float InRotationSpeed = -1.0f);
	void SetFacingDirection(const FVector& InFacingDirection, float InRotationSpeed = -1.0f);

	void SetForcedRotation(const FRotator& NewRotation);

	TEnumAsByte<ETraceTypeQuery> TraceType;

private:
	void Internal_SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed);
	FVector GetMovementDelta(const FGP3FrameMovement& FrameMovement) const;

	FRotator FacingRotationCurrent;
	FRotator FacingRotationTarget;
	float FacingRotationSpeed;

	float Radius;
	float HalfHeight;
	float SkinWidth = 0.5f;
};