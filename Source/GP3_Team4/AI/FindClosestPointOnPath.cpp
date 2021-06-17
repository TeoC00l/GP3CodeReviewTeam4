// Fill out your copyright notice in the Description page of Project Settings.


#include "FindClosestPointOnPath.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "GP3AINavMovementComponent.h"
#include "GP3AIStalkerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UFindClosestPointOnPath::UFindClosestPointOnPath()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

void UFindClosestPointOnPath::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();

	if(BlackboardKey.GetSelectedKeyID() == NULL)
	{
		return;
	}
	
	if(BlackboardKey.SelectedKeyType != UBlackboardKeyType_Vector::StaticClass())
	{
		return;
	}

	AGP3AIStalkerController* AICon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());
	FVector Position = AICon->GetPawn()->GetActorLocation();
	USplineComponent* Path = AICon->CurrentPath;

	float InputKey = Path->FindInputKeyClosestToWorldLocation(Position);
	float Distance = Path->GetDistanceAlongSplineAtSplineInputKey(InputKey);

	FVector TargetPosition = Path->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	UGP3AINavMovementComponent* NavMovementComponent = Cast<UGP3AINavMovementComponent>(AIPawn->GetMovementComponent());

	if(NavMovementComponent)
	{
		//SNAP TO FLOOR
		FNavLocation NavFloorLocation;

		if(NavMovementComponent->FindNavFloor(AIPawn->GetActorLocation(), NavFloorLocation))
		{
			TargetPosition.Z =  NavFloorLocation.Location.Z + AIPawn->GetDefaultHalfHeight();
		}
	}

	BlackboardComp->SetValueAsVector(BlackboardKey.SelectedKeyName, TargetPosition);

}
