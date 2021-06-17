// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3CharacterNoiseEmitter.h"


UGP3CharacterNoiseEmitter::UGP3CharacterNoiseEmitter()
{
}

void UGP3CharacterNoiseEmitter::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UE_LOG(LogTemp, Warning, TEXT("dskaopdsa"));
}
