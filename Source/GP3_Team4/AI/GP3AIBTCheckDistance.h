// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "GP3AIBTCheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM4_API UGP3AIBTCheckDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UGP3AIBTCheckDistance();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** Name of a key of type float to output the distance in */
	UPROPERTY(Category = Node, EditAnywhere)
	FName OutputKey;
	
	

};
