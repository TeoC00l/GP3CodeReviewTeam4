#include "GP3PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"

AGP3PlayerCharacter::AGP3PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
}

void AGP3PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGP3PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGP3PlayerCharacter::HandleMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGP3PlayerCharacter::HandleMoveRight);
	PlayerInputComponent->BindAxis("LookRight", this, &AGP3PlayerCharacter::HandleLookRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AGP3PlayerCharacter::HandleLookUp);

}

void AGP3PlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AGP3PlayerCharacter::HandleMoveForward(float Value)
{
	if(Value == 0.0f)
	{
		return;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator Yaw(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value * MovementSpeed);
}

void AGP3PlayerCharacter::HandleMoveRight(float Value)
{
	if(Value == 0.0f)
	{
		return;
	}
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator Yaw(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value * MovementSpeed);
}

void AGP3PlayerCharacter::HandleLookRight(float Value)
{
	AddControllerYawInput(LookSensitivity * GetWorld()->GetDeltaSeconds() * Value);
}

void AGP3PlayerCharacter::HandleLookUp(float Value)
{
	AddControllerPitchInput(LookSensitivity * GetWorld()->GetDeltaSeconds() * Value);
}
