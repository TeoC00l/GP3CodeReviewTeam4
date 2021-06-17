#pragma once

#include "GameFramework/GameState.h"
#include "GP3GameState.generated.h"

UCLASS()
class GP3_TEAM4_API AGP3GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void SetCurrentCheckpoint(FVector Pos, FRotator Rot);
	FVector GetCurrentCheckpointPos() const;
	FRotator GetCurrentCheckpointRot() const;
	
private:
	FVector CurrentCheckpointPos = FVector::ZeroVector;
	FRotator CurrentCheckpointRot = FRotator::ZeroRotator;
};
