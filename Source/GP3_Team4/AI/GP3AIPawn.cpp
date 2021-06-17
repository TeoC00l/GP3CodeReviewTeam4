
#include "GP3AIPawn.h"
#include "GP3AINavMovementComponent.h"


// Sets default values
AGP3AIPawn::AGP3AIPawn()
{
	AINavMovementComponent = CreateDefaultSubobject<UGP3AINavMovementComponent>(TEXT("AINavMovementComponent"));
}

void AGP3AIPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AGP3AIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

