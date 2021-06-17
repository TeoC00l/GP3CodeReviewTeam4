// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3AIBTSetBool.h"
#include "GP3AIStalkerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

EBTNodeResult::Type UGP3AIBTSetBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if(BlackboardKey.SelectedKeyType == nullptr || BlackboardKey.SelectedKeyType != UBlackboardKeyType_Bool::StaticClass())
	{
		return EBTNodeResult::Failed;
	}

	BlackboardComp->SetValueAsBool(GetSelectedBlackboardKey(), SetToTrue);

	return EBTNodeResult::Succeeded;
}
