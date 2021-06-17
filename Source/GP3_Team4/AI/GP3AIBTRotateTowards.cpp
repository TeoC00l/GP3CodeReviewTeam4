// Fill out your copyright notice in the Description page of Project Settings.

#include "GP3AIBTRotateTowards.h"
#include "GP3AIPawnStalker.h"
#include "GP3AIStalkerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/KismetMathLibrary.h"


UGP3AIBTRotateTowards::UGP3AIBTRotateTowards()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
	RotationSpeed = 3.f;
}

EBTNodeResult::Type UGP3AIBTRotateTowards::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGP3AIStalkerController* AIcon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());

	if(!AIcon)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
	}
	
	UBlackboardComponent* BlackboardComp = AIcon->GetBlackBoardComp();
	UObject* KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
	AGP3AIPawn* AI = Cast<AGP3AIPawnStalker>(AIcon->GetPawn());

	AActor* TargetActor = Cast<AActor>(KeyValue);

	FVector TargetLocation = FVector::ZeroVector;

	if(TargetActor)
	{
		TargetLocation = TargetActor->GetActorLocation();
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		TargetLocation = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
	}

	if (TargetLocation != FVector::ZeroVector)
	{
		CurrentQuat = AI->GetActorQuat();
		FVector Vec = (TargetLocation - AIcon->GetPawn()->GetActorLocation());
		Vec.Z = 0.f;
		FVector VecNormal = Vec.GetSafeNormal();

		TargetQuat = VecNormal.Rotation().Quaternion();
	}

	Alpha = 0.f;
	
	return EBTNodeResult::InProgress;
}

void UGP3AIBTRotateTowards::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Alpha += RotationSpeed * DeltaSeconds;
	FQuat Quat = FQuat::Slerp(CurrentQuat, TargetQuat, Alpha);
	
	AGP3AIStalkerController* AIcon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());

	AActor* AI = Cast<AActor>(AIcon->GetPawn());
	AI->SetActorRotation( FQuat::Slerp(CurrentQuat, TargetQuat, Alpha) );
	
	if(Alpha >= 1.f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
