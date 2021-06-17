// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3AIBTExitAttackMode.h"

#include "GP3AIStalkerController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UGP3AIBTExitAttackMode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGP3AIStalkerController* AIcon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());
	
	if(!AIcon)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = AIcon->GetBlackBoardComp();

	if(!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;

	//BlackboardComp->SetValueAsBool("bChasePlayer", False);
}
