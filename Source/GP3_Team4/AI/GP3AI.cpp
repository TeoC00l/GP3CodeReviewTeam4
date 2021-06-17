// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3AI.h"
// Sets default values
AGP3AI::AGP3AI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGP3AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AGP3AI::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void AGP3AI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
