#include "GP3InteractionBlueprint.h"

#include "GP3_Team4/PlayerCharacter/GP3PawnPlayer.h"

void UGP3InteractionBlueprint::Interact(AGP3PawnPlayer* PawnPlayer)
{
	if(IsOneTimeOnly && HasBeenUsed)
	{
		return;
	}
	HasBeenUsed = true;
	Interaction.Broadcast(PawnPlayer);
}

