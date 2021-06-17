#pragma once

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GP3PlayerCharacter.generated.h" 

class USpringArmComponent;
class USphereComponent;

UCLASS()
class AGP3PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGP3PlayerCharacter();

	virtual void BeginPlay() override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

public:

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;
	
private:
	void HandleMoveForward(float Value);
	void HandleMoveRight(float Value);
	void HandleLookRight(float Value);
	void HandleLookUp(float Value);

	UPROPERTY(EditDefaultsOnly)
	float LookSensitivity = 45.f;

	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly)
	float GravityStrength = 981.f;

	float ForwardInput = 0.f;
	float RightInput = 0.f;

	FVector Velocity;
	
};