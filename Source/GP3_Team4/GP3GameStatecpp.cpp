#include "GP3GameState.h"

void AGP3GameState::BeginPlay()
{
	
}

void AGP3GameState::SetCurrentCheckpoint(FVector Pos, FRotator Rot)
{
	CurrentCheckpointPos = Pos;
	CurrentCheckpointRot = Rot;

	UE_LOG(LogTemp, Log, TEXT("Checkpoint pos : %f, %f, %f"), Pos.X, Pos.Y, Pos.Z);
}

FVector AGP3GameState::GetCurrentCheckpointPos() const
{
	return CurrentCheckpointPos;
}

FRotator AGP3GameState::GetCurrentCheckpointRot() const
{
	return CurrentCheckpointRot;
}
