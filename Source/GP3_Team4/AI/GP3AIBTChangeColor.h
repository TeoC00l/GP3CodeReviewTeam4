// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GP3AIBTChangeColor.generated.h"

UENUM(BlueprintType)
enum AIColor
{
  PURPLE     UMETA(DisplayName = "Purple"),
  ORANGE      UMETA(DisplayName = "Orange"),
  RED   UMETA(DisplayName = "Red"),
};

/**
 * 
 */
 
UCLASS()
class GP3_TEAM4_API UGP3AIBTChangeAIColor : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGP3AIBTChangeAIColor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Color)
	TEnumAsByte<AIColor> Color;

	UFUNCTION(BlueprintCallable)
	AIColor GetColor();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
