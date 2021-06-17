
#include "GP3AIStalker.h"
#include "GP3AIStalkerController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

AGP3AIStalker::AGP3AIStalker()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->SensingInterval = .25f;
	PawnSensingComp->SetPeripheralVisionAngle(80.f);
	PawnSensingComp->bHearNoises = true;
}

void AGP3AIStalker::BeginPlay()
{
	Super::BeginPlay();

	if(PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AGP3AIStalker::OnSeePawn);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AGP3AIStalker::OnHearNoise);
	}

}


void AGP3AIStalker::OnSeePawn(APawn* Pawn)
{
	 AGP3AIStalkerController* AIController = Cast<AGP3AIStalkerController>(GetController());

	if(AIController)
	 {
	 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PURSUING PLAYER!!"));
	 	AIController->SetPlayerCaught(Pawn);
	 }
}

void AGP3AIStalker::OnHearNoise(APawn* Pawn, const FVector& Location, float Volume)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI heard something!"));
}
