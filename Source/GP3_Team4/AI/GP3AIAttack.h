// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3AIAttack.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM4_API UGP3AIAttack : public UBTTaskNode
{
	GENERATED_BODY()

	UGP3AIAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
