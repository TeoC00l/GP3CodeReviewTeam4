// Fill out your copyright notice in the Description page of Project Settings.

#include "GP3AIBTStopMovement.h"
#include "GP3AINavMovementComponent.h"
#include "GP3AIPawn.h"
#include "GP3AIPawnStalker.h"
#include "GP3AIStalkerController.h"

EBTNodeResult::Type UGP3AIBTStopMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGP3AIStalkerController* AIcon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());

	UBlackboardComponent* BlackboardComp = AIcon->GetBlackBoardComp();

	AGP3AIPawn* AI = Cast<AGP3AIPawnStalker>(AIcon->GetPawn());

	if(AI)
	{
		UGP3AINavMovementComponent* NavMovementComponent = AI->AINavMovementComponent;
			
		if(NavMovementComponent)
		{
			NavMovementComponent->StopActiveMovement();
		}
	}

	return EBTNodeResult::Succeeded;
}
