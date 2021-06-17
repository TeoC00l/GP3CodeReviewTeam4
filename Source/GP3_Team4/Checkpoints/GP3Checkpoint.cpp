#include "GP3Checkpoint.h"
#include "Components/BoxComponent.h"
#include "GP3_Team4/GP3GameState.h"
#include "GP3_Team4/PlayerCharacter/GP3PawnPlayer.h"

AGP3Checkpoint::AGP3Checkpoint()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;
	
	Checkpoint = CreateDefaultSubobject<USceneComponent>(TEXT("Checkpoint"));
	Checkpoint->SetupAttachment(RootComp);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	Box->SetupAttachment(RootComp);
}

void AGP3Checkpoint::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AGP3Checkpoint::OnBeginOverlap);
}

void AGP3Checkpoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Checkpoint reached"));
	
	AGP3GameState* GameState = Cast<AGP3GameState>(GetWorld()->GetGameState());

	if(!GameState)
	{
		UE_LOG(LogTemp, Log, TEXT("Checkpoint couldn't find the gamestate"));
		return;
	}

	GameState->SetCurrentCheckpoint(Checkpoint->GetComponentLocation(), Checkpoint->GetComponentRotation());
	OnCheckpointReached();
}
