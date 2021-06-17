// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3AIBTPerformSearch.generated.h"

class AGP3AIPawnStalker;

struct GP3AIBTPerformSearchMemory
{
	/** time left */
};

UCLASS()
class GP3_TEAM4_API UGP3AIBTPerformSearch : public UBTTaskNode
{
	GENERATED_BODY()

//CONSTRUCTORS
	UGP3AIBTPerformSearch();

	
//VARIABLES
public:
	UPROPERTY(Category = Search, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SearchDistance;
	
	UPROPERTY(Category = Search, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SearchTimePerState;
	
	UPROPERTY(Category = Search, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SearchPadding;

private:
	float RemainingSearchTime;
	int CurrentSearchState;
	float LerpTimeElapsed;
	bool bExtendingForward = true;

	UPROPERTY(Transient)
	AGP3AIPawnStalker* PawnStalker;
	
//METHODS
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
