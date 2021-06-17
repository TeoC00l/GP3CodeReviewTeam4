// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GP3AIBTRotateTowards.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM4_API UGP3AIBTRotateTowards : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UGP3AIBTRotateTowards();

	float Alpha;

	/** Name of a key of type float to output the distance in */
	UPROPERTY(Category = Node, EditAnywhere)
	float RotationSpeed;

	UPROPERTY(Transient)
	FQuat TargetQuat;

	UPROPERTY(Transient)
	FQuat CurrentQuat;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
