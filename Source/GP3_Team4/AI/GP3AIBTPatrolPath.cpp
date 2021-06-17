// Fill out your copyright notice in the Description page of Project Settings.

#include "GP3AIBTPatrolPath.h"
#include "GP3AIStalkerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GP3AINavMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGP3AIBTPatrolPath::UGP3AIBTPatrolPath()
{
	PatrollingSpeed = 200.f;
	bGoingForward = true;

	bCreateNodeInstance = true;
	bNotifyTick = true;
}


EBTNodeResult::Type UGP3AIBTPatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	if(!AICon)
	{
		AICon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());
	}
	
	if(!BlackBoardComp)
	{
		BlackBoardComp = AICon->GetBlackBoardComp();
	}
	
	if(!AINavMovementComponent)
	{
		AINavMovementComponent = Cast<UGP3AINavMovementComponent>(AICon->GetPawn()->GetMovementComponent());
	}

	if(AINavMovementComponent)
	{
		AINavMovementComponent->bHasRequestedVelocity = false;
	}

	PatrollingSpeed = AINavMovementComponent->PatrolSpeed;

	if(MovementMagnitude == 0.f)
	{
		MovementMagnitude = PatrollingSpeed;
	}

	if(AICon->CurrentPath == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller ' %s ' has no path splines defined"), *AICon->GetName());
		return EBTNodeResult::Failed;
	}

	FVector Position = BlackBoardComp->GetValueAsVector("ClosestPointOnPath");

	if(Position != FVector::ZeroVector)
	{
		float InputKey = AICon->CurrentPath->FindInputKeyClosestToWorldLocation(Position);
		float Distance = AICon->CurrentPath->GetDistanceAlongSplineAtSplineInputKey(InputKey) + MovementMagnitude*GetWorld()->DeltaTimeSeconds;
		BlackBoardComp->SetValueAsFloat("DistanceAlongPath", Distance);
		
	}

	AICon->MoveToLocation(AICon->GetPawn()->GetActorLocation(), 500.f, true);
	AICon->StopMovement();

	//HINDER WEIRD ROTATION SCENARIOS
	
	float DistanceAlongPath = BlackBoardComp->GetValueAsFloat("DistanceAlongPath");

	if(!AICon->CurrentPath->IsClosedLoop())
	{
		if(DistanceAlongPath+MovementMagnitude > AICon->CurrentPath->GetSplineLength() || DistanceAlongPath+MovementMagnitude < 0)
		{
			TurnAround();
			FTransform PathTransform = AICon->CurrentPath->GetTransformAtDistanceAlongSpline(DistanceAlongPath + MovementMagnitude * GetWorld()->DeltaTimeSeconds, ESplineCoordinateSpace::World);
			FVector TurnTo = PathTransform.GetTranslation();
			BlackBoardComp->SetValueAsVector("LastPatrolPosition", TurnTo);

			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::InProgress;
}

void UGP3AIBTPatrolPath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
	float DistanceAlongPath = BlackBoardComp->GetValueAsFloat("DistanceAlongPath") + (MovementMagnitude * GetWorld()->DeltaTimeSeconds);
	BlackBoardComp->SetValueAsFloat("DistanceAlongPath", (DistanceAlongPath));

	USplineComponent* CurrentPath = AICon->CurrentPath;

	if(DistanceAlongPath > CurrentPath->GetSplineLength() || DistanceAlongPath < 0)
	{
		TurnAround();

		if(!CurrentPath->IsClosedLoop())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	
	FTransform PathTransform = CurrentPath->GetTransformAtDistanceAlongSpline(DistanceAlongPath, ESplineCoordinateSpace::World);
	FVector Position = PathTransform.GetTranslation();

	APawn* Pawn = AICon->GetPawn();
	AActor* Act = Cast<AActor>(AICon->GetPawn());

	BlackBoardComp->SetValueAsVector("LastPatrolPosition", Pawn->GetActorLocation());

	INavigationDataInterface* NavData = FNavigationSystem::GetNavDataForActor(*Act);
	FNavLocation OutLocation;

	const FVector ProjectionTarget = Position - FVector(0.f,0.f, Pawn->GetDefaultHalfHeight());

	const FNavAgentProperties& AgentProps = Pawn->GetMovementComponent()->GetNavAgentPropertiesRef();

	if(AgentProps == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller ' %s ' has associated FNavAgentProperties"), *AICon->GetName());
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float SearchRadius = AgentProps.AgentRadius * 2.0f;
	const float SearchHeight = AgentProps.AgentHeight * AgentProps.NavWalkingSearchHeightScale;

	if(NavData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller ' %s ' has associated NavData"), *AICon->GetName());
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	if(NavData->ProjectPoint(ProjectionTarget, OutLocation, FVector(SearchRadius,SearchRadius,SearchHeight)))
	{
		float FloorHeight = OutLocation.Location.Z;
		FVector CurrentLocation = Position;
		CurrentLocation.Z = Pawn->GetActorLocation().Z;
		
		if(CurrentPath)
		{
			FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(Pawn->GetActorLocation(), CurrentLocation);
			AICon->GetPawn()->SetActorLocation(CurrentLocation);
			Pawn->SetActorRotation(PlayerRot);			
		}
	}
}


void UGP3AIBTPatrolPath::TurnAround()
{
	USplineComponent* CurrentPath = AICon->CurrentPath;

		if(CurrentPath->IsClosedLoop())
		{
			float DistanceAlongPath = BlackBoardComp->GetValueAsFloat("DistanceAlongPath");
			DistanceAlongPath -= CurrentPath->GetSplineLength();
			BlackBoardComp->SetValueAsFloat("DistanceAlongPath", DistanceAlongPath);

			//TODO: FIX TURNING AROUND THING
		}
		else if (MovementMagnitude>0)
		{
			MovementMagnitude = -PatrollingSpeed;
			float DistanceAlongPath = BlackBoardComp->GetValueAsFloat("DistanceAlongPath") + (MovementMagnitude * GetWorld()->DeltaTimeSeconds);
			BlackBoardComp->SetValueAsFloat("DistanceAlongPath", (DistanceAlongPath));
		}else
		{
			MovementMagnitude = PatrollingSpeed;
			float DistanceAlongPath = BlackBoardComp->GetValueAsFloat("DistanceAlongPath") + (MovementMagnitude * GetWorld()->DeltaTimeSeconds);
			BlackBoardComp->SetValueAsFloat("DistanceAlongPath", (DistanceAlongPath));

			// FVector NextRotation = AICon->GetPawn()->GetActorRotation().Vector();
			// NextRotation.Z = NextRotation.Z = -NextRotation.Z;
			// BlackBoardComp->SetValueAsVector("TurningAroundDirection", (NextRotation));
		}
}