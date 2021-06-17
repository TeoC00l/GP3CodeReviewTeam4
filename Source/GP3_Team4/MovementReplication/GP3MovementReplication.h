#pragma once

#include "Components/ActorComponent.h"
#include "GP3MovementReplication.generated.h"

UCLASS(Meta = (BlueprintSpawnableComponent))
class UGP3MovementReplication : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	AActor* ConnectedActor;

	UFUNCTION(BlueprintCallable)
	void CreateConnection(AActor* Connection);

	UFUNCTION(BlueprintCallable)
	void SetPartnerAsParent();

	UFUNCTION(BlueprintCallable)
	void SetPartnerAsChild();

	UFUNCTION(BlueprintCallable)
	void DetachFromParent();

	UFUNCTION(BlueprintCallable)
	void SetPartnerOffset();

	UFUNCTION(BlueprintCallable)
	void ApplyPositionOffset();

	UFUNCTION(BlueprintCallable)
	void ApplyOffsetToPartner();

	UFUNCTION(BlueprintCallable)
	void BreakPartnerConnection();

private:
	UPROPERTY(VisibleAnywhere)
	FVector OffsetVector = FVector::ZeroVector;
};