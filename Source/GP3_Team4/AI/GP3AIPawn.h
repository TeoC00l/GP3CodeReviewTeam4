// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GP3AIPawn.generated.h"

UCLASS()
class GP3_TEAM4_API AGP3AIPawn : public APawn
{
GENERATED_BODY()

//CONSTRUCTORS
public:
	AGP3AIPawn();

//VARIABLES
public:
	UPROPERTY(EditAnywhere, Category= AI)
	class UBehaviorTree* BehaviourTree;

	UPROPERTY(VisibleAnywhere, Category=Sensing)
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleDefaultsOnly, Category=Movement)
	class UGP3AINavMovementComponent* AINavMovementComponent;


//METHODS:
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:

};
