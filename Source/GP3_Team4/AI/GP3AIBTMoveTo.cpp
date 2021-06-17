// Fill out your copyright notice in the Description page of Project Settings.

#include "GP3AIBTMoveTo.h"
#include "GP3AINavMovementComponent.h"
#include "GP3AIPawnStalker.h"
#include "GP3AIStalkerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UGP3AIBTMoveTo::UGP3AIBTMoveTo()
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
	StopOnOverlap = 1;

}

EBTNodeResult::Type UGP3AIBTMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UGP3AIBTMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AGP3AIStalkerController* AIcon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());
		
	if(AIcon)
	{
		UBlackboardComponent* BlackboardComp = AIcon->GetBlackBoardComp();

		AGP3AIPawn* AI = Cast<AGP3AIPawnStalker>(AIcon->GetPawn());

		if(AI)
		{
			UGP3AINavMovementComponent* NavMovementComponent = AI->AINavMovementComponent;
			
			if(NavMovementComponent)
			{
				NavMovementComponent->bHasRequestedVelocity = true;

				UObject* KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
				AActor* TargetActor = Cast<AActor>(KeyValue);

				FVector TargetLocation = FVector::ZeroVector;
				
				if(TargetActor)
				{
					TargetLocation = TargetActor->GetActorLocation();

					AIcon->MoveToActor(TargetActor);

					float DistSq = FVector::DistSquared(TargetLocation, AIcon->GetPawn()->GetActorLocation());
				
					if(DistSq <= (AcceptableRadius*AcceptableRadius + 1.f))
					{
						NavMovementComponent->bHasRequestedVelocity = false;
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					}
				}				
				else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
				{
					TargetLocation = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
				}

				if (TargetLocation != FVector::ZeroVector)
				{
					FVector LookAtPosition = FVector(TargetLocation.X, TargetLocation.Y, AI->GetActorLocation().Z);
					FVector Velocity = NavMovementComponent->RequestedVelocity;

					FQuat CurrentRot = AI->GetActorQuat();
					FQuat TargetRot = Velocity.GetSafeNormal().Rotation().Quaternion();
					
					AI->SetActorRotation(FQuat::Slerp(CurrentRot, TargetRot, DeltaSeconds * 5.f));

					if(StopOnOverlap)
					{
						AIcon->MoveToLocation(TargetLocation, 0.f, true, true);
					}else
					{
						AIcon->MoveToLocation(TargetLocation, 0.f, false, true);
					}
					
					float DistSq = FVector::DistSquared(TargetLocation, AIcon->GetPawn()->GetActorLocation());
				
					if(DistSq <= (AcceptableRadius*AcceptableRadius + 1.f))
					{
						NavMovementComponent->bHasRequestedVelocity = false;
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					}
				}
			}
		}
	}
}
