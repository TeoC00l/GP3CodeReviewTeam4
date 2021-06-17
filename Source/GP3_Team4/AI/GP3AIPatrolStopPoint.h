
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP3AIPatrolStopPoint.generated.h"

UCLASS()
class GP3_TEAM4_API AGP3AIPatrolStopPoint : public AActor
{
	GENERATED_BODY()

//CONSTRUCTORS
public:	
	AGP3AIPatrolStopPoint();

//VARIABLES
public:
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;
	
//METHODS
public:	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


};
