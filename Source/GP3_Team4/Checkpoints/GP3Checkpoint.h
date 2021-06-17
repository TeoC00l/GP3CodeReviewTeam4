#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP3Checkpoint.generated.h"

UCLASS()
class GP3_TEAM4_API AGP3Checkpoint : public AActor
{
	GENERATED_BODY()

public:
	AGP3Checkpoint();
	virtual void BeginPlay() override;

	UPROPERTY()
	class USceneComponent* RootComp;
	
	UPROPERTY(EditInstanceOnly)
	class UBoxComponent* Box;
	
	UPROPERTY(EditInstanceOnly)
	class USceneComponent* Checkpoint;

	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCheckpointReached();
};
