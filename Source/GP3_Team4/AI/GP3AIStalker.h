
#pragma once

#include "CoreMinimal.h"
#include "GP3AI.h"
#include "GP3AIStalker.generated.h"

class USplineComponent;

UCLASS()
class GP3_TEAM4_API AGP3AIStalker : public AGP3AI
{
	GENERATED_BODY()

//CONSTRUCTORS
AGP3AIStalker();
	
//VARIABLES
public:
	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> PathActors;
	UPROPERTY(Transient)
	TArray<USplineComponent*> PathSplines;
	
//FUNCTIONS
public:
	UFUNCTION()
    void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* Pawn, const FVector &Location, float Volume);

	protected:
	virtual void BeginPlay() override;
};
