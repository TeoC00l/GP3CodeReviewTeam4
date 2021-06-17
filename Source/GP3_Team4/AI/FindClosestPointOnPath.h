// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "FindClosestPointOnPath.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM4_API UFindClosestPointOnPath : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
	
	public:
	UFindClosestPointOnPath();
	
	protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
