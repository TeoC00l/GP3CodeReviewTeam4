#pragma once

#include "Components/ActorComponent.h"
#include "GP3InteractionBase.generated.h"

class AGP3PawnPlayer;

UCLASS(Abstract)
class UGP3InteractionBase : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void Interact(AGP3PawnPlayer* PawnPlayer){}

	virtual void StopInteraction(AGP3PawnPlayer* PawnPlayer){}

	virtual bool AllowDropOff(){return true;}
};