// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3AIBTChangeColor.h"
#include "GP3AIPawnStalker.h"
#include "GP3AIStalkerController.h"

UGP3AIBTChangeAIColor::UGP3AIBTChangeAIColor()
{
	bCreateNodeInstance = true;
	Color = PURPLE;
}

AIColor UGP3AIBTChangeAIColor::GetColor()
{
	return Color;
}

EBTNodeResult::Type UGP3AIBTChangeAIColor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AGP3AIStalkerController* AIcon = Cast<AGP3AIStalkerController>(OwnerComp.GetAIOwner());

	if(!AIcon)
	{
		return EBTNodeResult::Failed;
	}

	AGP3AIPawnStalker* PawnStalker = Cast<AGP3AIPawnStalker>(AIcon->GetPawn());

	if(!PawnStalker)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = AIcon->GetBlackBoardComp();


	if(!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	PawnStalker->Color = Color;
	PawnStalker->ChangeColorEvent.Broadcast(Color);

	return EBTNodeResult::Succeeded;
}
