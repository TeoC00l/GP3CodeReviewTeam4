// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GP3CharacterNoiseEmitter.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM4_API UGP3CharacterNoiseEmitter : public UPawnNoiseEmitterComponent
{
	GENERATED_BODY()

//CONSTRUCTORS:
	UGP3CharacterNoiseEmitter();

//VARIABLES

//METHODS
	public:
	virtual void TickComponent
    (
        float DeltaTime,
        enum ELevelTick TickType,
        FActorComponentTickFunction * ThisTickFunction
    ) override;
};
