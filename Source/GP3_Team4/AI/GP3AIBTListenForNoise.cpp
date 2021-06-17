// Fill out your copyright notice in the Description page of Project Settings.

#include "GP3AIBTListenForNoise.h"

#include "BehaviorTree/BlackboardComponent.h"

UGP3AIBTListenForNoise::UGP3AIBTListenForNoise()
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UGP3AIBTListenForNoise::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Timer = 0.f;
	return EBTNodeResult::InProgress;
}

void UGP3AIBTListenForNoise::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Timer+=DeltaSeconds;

	if(Timer >= Duration)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	float TimeSinceNoise = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("TimeSinceHeardNoise");

	if(TimeSinceNoise < 0.1f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("bChasePlayer", true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
