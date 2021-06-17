#include "GP3PortalMiniGameHandler.h"
#include "GP3_Team4/PlayerCharacter/GP3PawnPlayer.h"
#include "Camera/CameraComponent.h"
#include "GP3_Team4/Interaction/GP3InteractionBlueprint.h"
#include "GP3PortalPiece.h"

AGP3PortalMiniGameHandler::AGP3PortalMiniGameHandler()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>("CollisionBox");
	RootComponent = Root;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);

	InteractionComponent = CreateDefaultSubobject<UGP3InteractionBlueprint>("InteractionComponent");

}

void AGP3PortalMiniGameHandler::Tick(float DeltaSeconds)
{
	if(CurrentPiece)
	{
		CurrentPiece->MoveInDirection(MouseInput);
		CheckDistance();
	}
}

void AGP3PortalMiniGameHandler::BeginPlay()
{
	Super::BeginPlay();

	GetComponents<UGP3PortalPiece>(PortalPieces);
}

void AGP3PortalMiniGameHandler::BecomeControlledPawn(AGP3PawnPlayer* InteractingPlayer)
{
	if(IsOpened)
	{
		return;
	}
	CurrentIndex = 0;

	for (UGP3PortalPiece* Piece : PortalPieces)
	{
		if(Piece->IsUsable)
		{
			CurrentPiece = Piece;
			break;
		}
		CurrentIndex ++;
	}
	
	Player = InteractingPlayer;
	Player->SetMovementLock(true);
	SetActorTickEnabled(true);
	PossessTarget.Broadcast(this);
	//InteractingPlayer->Controller->Possess(this);
}

void AGP3PortalMiniGameHandler::ResetPuzzle()
{
	for (UGP3PortalPiece* Piece : PortalPieces)
	{
		Piece->Reset();
	}
}

void AGP3PortalMiniGameHandler::ActivatePuzzle()
{
	for (UGP3PortalPiece* Piece : PortalPieces)
	{
		Piece->SetActiveState(true);
	}
}

void AGP3PortalMiniGameHandler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AGP3PortalMiniGameHandler::HandleInteract);

	PlayerInputComponent->BindAxis("LookRight", this, &AGP3PortalMiniGameHandler::HandleHorizontal);
	PlayerInputComponent->BindAxis("LookUp", this, &AGP3PortalMiniGameHandler::HandleVertical);
}

void AGP3PortalMiniGameHandler::HandleHorizontal(float Input)
{
	MouseInput.X = Input;
}

void AGP3PortalMiniGameHandler::HandleVertical(float Input)
{
	MouseInput.Y = -Input;
}

void AGP3PortalMiniGameHandler::HandleInteract()
{
	PossessPlayer.Broadcast(Player);
	Player->SetMovementLock(false);
	SetActorTickEnabled(false);
}

void AGP3PortalMiniGameHandler::CheckDistance()
{
	float CurrentDist = FVector::DistSquared(CurrentPiece->GetComponentLocation(), GetActorLocation());
	if(CurrentDist >= DistanceFromCenter * DistanceFromCenter)
	{
		CurrentPiece->FinishMovement();
		
		if(++CurrentIndex < PortalPieces.Num())
		{
			CurrentPiece = PortalPieces[CurrentIndex];
		}
		else
		{
			IsOpened = true;
			HandleInteract();
		}
	}
}


