#pragma once

#include "GP3InteractionBase.h"
#include "GP3InteractionBlueprint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteract, AGP3PawnPlayer*, Player);

UCLASS(Meta = (BlueprintSpawnableComponent), DisplayName = "InteractionBlueprint")
class UGP3InteractionBlueprint : public UGP3InteractionBase
{
	GENERATED_BODY()
public:
	virtual void Interact(AGP3PawnPlayer* PawnPlayer) override;

	UPROPERTY(BlueprintAssignable)
	FInteract Interaction;

	UPROPERTY(EditAnywhere)
	bool IsOneTimeOnly = false;

private:
	bool HasBeenUsed = false;

};