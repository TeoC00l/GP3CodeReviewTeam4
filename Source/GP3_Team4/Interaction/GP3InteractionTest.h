#pragma once

#include "GP3InteractionBase.h"
#include "GP3InteractionTest.generated.h"


UCLASS(Meta = (BlueprintSpawnableComponent))
class UGP3InteractionTest : public UGP3InteractionBase
{
	GENERATED_BODY()

public:
	virtual void Interact(AGP3PawnPlayer* PawnPlayer) override;

	virtual void StopInteraction(AGP3PawnPlayer* PawnPlayer) override;
};