// Fill out your copyright notice in the Description page of Project Settings.

#include "GP3AIBTAddTickToBBKeyFloat.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

UGP3AIBTAddTickToBBKeyFloat::UGP3AIBTAddTickToBBKeyFloat()
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

void UGP3AIBTAddTickToBBKeyFloat::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if(BlackboardKey.SelectedKeyType != UBlackboardKeyType_Float::StaticClass())
	{
		return;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();

	if(BlackboardKey.GetSelectedKeyID() == NULL)
	{
		return;
	}
	
	if(BlackboardComp->GetKeyType( BlackboardKey.GetSelectedKeyID() ) != UBlackboardKeyType_Float::StaticClass()) return;

	
	float KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Float>(BlackboardKey.GetSelectedKeyID());

	KeyValue+=DeltaSeconds;

	
	BlackboardComp->SetValueAsFloat( GetSelectedBlackboardKey(), KeyValue);
}
