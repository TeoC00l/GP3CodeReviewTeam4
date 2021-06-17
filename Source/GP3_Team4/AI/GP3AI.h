// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GP3AI.generated.h"

UCLASS(Abstract)
class GP3_TEAM4_API AGP3AI : public ACharacter
{
	GENERATED_BODY()

//CONSTRUCTORS
public:
	AGP3AI();

//VARIABLES
public:
	UPROPERTY(EditAnywhere, Category= AI)
	class UBehaviorTree* BehaviourTree;

	UPROPERTY(VisibleAnywhere, Category=AI)
	class UPawnSensingComponent* PawnSensingComp;
	
//METHODS:
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
private:

};
