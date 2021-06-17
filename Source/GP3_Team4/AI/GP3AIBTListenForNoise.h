// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3AIBTListenForNoise.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM4_API UGP3AIBTListenForNoise : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UGP3AIBTListenForNoise();

	UPROPERTY(Category = Node, EditAnywhere, meta=(ClampMin = "0.0", UIMin="0.0"))
	float Duration;


protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float Timer;

};
