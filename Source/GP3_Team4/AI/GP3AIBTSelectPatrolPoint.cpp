// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3AIBTSelectPatrolPoint.h"
#include "GP3AIPatrolPoint.h"
#include "GP3AIStalkerController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UGP3AIBTSelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGP3AIStalkerController* AIcon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());

	//DEPRECIATED
	
	// if(AIcon)
	// {
	// 	UBlackboardComponent* BlackboardComp = AIcon->GetBlackBoardComp();
	// 	AGP3AIPatrolPoint* CurrentPoint = Cast<AGP3AIPatrolPoint>(BlackboardComp->GetValueAsObject("LocationToGo"));
	//
	// 	TArray<AActor*> AvailablePatrolPoints = AIcon->GetPatrolPoints();
	//
	// 	AGP3AIPatrolPoint* NextPatrolPoint = nullptr;
	//
	// 	if(AIcon->CurrentPatrolPoint != AvailablePatrolPoints.Num() -1)
	// 	{
	// 		NextPatrolPoint = Cast<AGP3AIPatrolPoint>(AvailablePatrolPoints[++AIcon->CurrentPatrolPoint]);
	// 	}else
	// 	{
	// 		NextPatrolPoint = Cast<AGP3AIPatrolPoint>(AvailablePatrolPoints[0]);
	// 		AIcon->CurrentPatrolPoint = 0;
	// 	}
	//
	// 	BlackboardComp->SetValueAsObject("LocationToGo", NextPatrolPoint);
	//
	// 	return EBTNodeResult::Succeeded;
	// }

	return EBTNodeResult::Failed;

}
