// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GP3AIStalkerController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3AIBTSelectPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM4_API UGP3AIBTSelectPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
