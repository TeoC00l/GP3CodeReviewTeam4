// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GP3AIBTAddTickToBBKeyFloat.generated.h"

UCLASS()
class GP3_TEAM4_API UGP3AIBTAddTickToBBKeyFloat : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UGP3AIBTAddTickToBBKeyFloat();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
