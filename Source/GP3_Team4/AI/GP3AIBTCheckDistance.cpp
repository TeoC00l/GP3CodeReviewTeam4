// Fill out your copyright notice in the Description page of Project Settings.

#include "GP3AIBTCheckDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UGP3AIBTCheckDistance::UGP3AIBTCheckDistance()
{
	bCreateNodeInstance = true;
	bRestartTimerOnEachActivation = true;
}

void UGP3AIBTCheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	const FBlackboard::FKey KeyID = OwnerComp.GetBlackboardComponent()->GetKeyID(OutputKey);
	
	if(KeyID == FBlackboard::InvalidKey || KeyID == NULL) return;
	if(BlackboardComp->GetKeyType(KeyID) != UBlackboardKeyType_Float::StaticClass()) return;
	
	float KeyValue = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(OutputKey);
	
	UObject* TargetKeyObject = BlackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());

	FVector TargetLocation = FVector::ZeroVector;
				
	if(BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));

		if(TargetActor)
		{
			TargetLocation = TargetActor->GetActorLocation();
		}
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		TargetLocation = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
	}

	const float Distance = FVector::Distance(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), TargetLocation);

	 if(Distance)
	 {
	 	BlackboardComp->SetValueAsFloat(OutputKey, Distance);
	 }
}
