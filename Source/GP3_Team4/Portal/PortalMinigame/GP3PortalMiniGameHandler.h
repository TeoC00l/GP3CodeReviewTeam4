#pragma once

#include "GameFramework/Pawn.h"
#include "GP3PortalMiniGameHandler.generated.h"

class AGP3PawnPlayer;
class UGP3InteractionBlueprint;
class UCameraComponent;
class UBoxComponent;
class UGP3PortalPiece;
class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPossessTarget, AActor*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPossessPlayer, AGP3PawnPlayer*, PlayerTarget);

UCLASS()
class AGP3PortalMiniGameHandler : public APawn
{
	GENERATED_BODY()

public:

	AGP3PortalMiniGameHandler();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void BecomeControlledPawn(AGP3PawnPlayer* InteractingPlayer);

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	UGP3InteractionBlueprint* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, Category = Camera, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = Collision)
	USceneComponent* Root;

	UFUNCTION(BlueprintCallable)
	void ResetPuzzle();

	UFUNCTION(BlueprintCallable)
	void ActivatePuzzle();

	UPROPERTY(BlueprintAssignable)
	FPossessTarget PossessTarget;

	UPROPERTY(BlueprintAssignable)
	FPossessPlayer PossessPlayer;

private:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void HandleHorizontal(float Input);
	void HandleVertical(float Input);
	void HandleInteract();

	void CheckDistance();
		
	UPROPERTY(Transient)
	AGP3PawnPlayer* Player;

	TArray<UGP3PortalPiece*> PortalPieces;

	UPROPERTY(Transient)
	UGP3PortalPiece* CurrentPiece;

	UPROPERTY(EditAnywhere)
	float DistanceFromCenter = 150.f;
	
	bool IsOpened = false;
	int CurrentIndex = 0;

	FVector MouseInput;
};