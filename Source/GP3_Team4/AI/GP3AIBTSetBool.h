// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GP3AIBTSetBool.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM4_API UGP3AIBTSetBool : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	
	/** if checked the selected bool will be set to true. Else it will be set to false */
	UPROPERTY(Category = Node, EditAnywhere)
	uint32 SetToTrue : 1;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
};
