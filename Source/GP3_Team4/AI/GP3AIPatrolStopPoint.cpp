// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3AIPatrolStopPoint.h"
#include "Components/SphereComponent.h"

AGP3AIPatrolStopPoint::AGP3AIPatrolStopPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName(TEXT("Collision"));
	SphereComponent->SetupAttachment(RootComponent);
}

void AGP3AIPatrolStopPoint::BeginPlay()
{
	Super::BeginPlay();
}

void AGP3AIPatrolStopPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



