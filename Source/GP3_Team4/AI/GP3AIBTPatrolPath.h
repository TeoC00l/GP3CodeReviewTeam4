
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3AIBTPatrolPath.generated.h"

struct FGP3AIBTPatrolPathMemory
{
	float DistanceAlongPath;
	
};

/**
 * Move Along spline path
 */
UCLASS()
class GP3_TEAM4_API UGP3AIBTPatrolPath : public UBTTaskNode
{
	GENERATED_BODY()

//CONSTRUCTORS
public:
	UGP3AIBTPatrolPath();
	
//VARIABLES
public:
	float PatrollingSpeed;
	
private:
	UPROPERTY(Transient)
	class AGP3AIStalkerController* AICon;
	
	UPROPERTY(Transient)
	class UBlackboardComponent* BlackBoardComp;

	UPROPERTY(Transient)
	class UGP3AINavMovementComponent* AINavMovementComponent;

	UPROPERTY(Transient)
	class USplineComponent* PathSpline;

	bool bGoingForward;
	float MovementMagnitude;
	
//METHODS
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TurnAround();
};
