// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GP3AIBTMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM4_API UGP3AIBTMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UGP3AIBTMoveTo();

	/** fixed distance added to threshold between AI and goal location in destination reach test */
	UPROPERTY(Category = Node, EditAnywhere, meta=(ClampMin = "0.0", UIMin="0.0"))
	float AcceptableRadius;

	/** if checked the selected bool will be set to true. Else it will be set to false */
	UPROPERTY(Category = Node, EditAnywhere)
	uint32 StopOnOverlap : 1;
	

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
