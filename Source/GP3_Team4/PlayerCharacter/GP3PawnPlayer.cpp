#include "GP3PawnPlayer.h"

#include "DrawDebugHelpers.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GP3PlayerMovementComponent.h"
#include "GP3PlayerMovementStatistics.h"
#include "GP3_Team4/GP3GameState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GP3_Team4/Interaction/GP3InteractionBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GP3_Team4/Interaction/GP3InteractionPushAndPull.h"


AGP3PawnPlayer::AGP3PawnPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false; // this causes the mesh to rotate to the player's look direction at all times. Set to false if we wanna change the rotation later

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;
	// Set size for collision capsule
	Capsule->InitCapsuleSize(55.f, 96.0f);

	// This is the connection between the camera and the player, TargetArmLength indicates the distance from the player
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->bUsePawnControlRotation = true;


	// Create a CameraComponent	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	//Mesh->SetOnlyOwnerSee(true);
	Mesh->SetupAttachment(RootComponent);
	Mesh->bCastDynamicShadow = true;
	Mesh->CastShadow = true;
	Mesh->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	//movement component here
	MovementComponent = CreateDefaultSubobject<UGP3PlayerMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(Capsule);
}

float AGP3PawnPlayer::GetHorizontalSensitivity() const
{
	return HorizontalSensitivity;
}

float AGP3PawnPlayer::GetVerticalSensitivity() const
{
	return VerticalSensitivity;
}

void AGP3PawnPlayer::SetHorizontalSensitivity(float Value)
{
	HorizontalSensitivity = Value;
}

void AGP3PawnPlayer::SetVerticalSensitivity(float Value)
{
	VerticalSensitivity = Value;
}

void AGP3PawnPlayer::BlendCameraView(AActor* BlendTarget, float Duration)
{
	CameraBlend.Broadcast(BlendTarget, Duration);
}

void AGP3PawnPlayer::SetInteractionConnection(UGP3InteractionBase* InteractionPartner)
{
	ConnectedInteractionItem = InteractionPartner;
	HandleEndSprint();
	HandleEndCrouch();
	//PerformPush.Broadcast(true);
}

void AGP3PawnPlayer::SetMovementPartner(UGP3InteractionBase* InteractionPartner, float NewInteractionSpeed)
{
	PushAndPullSpeedReduction = NewInteractionSpeed;
	MovementPartner = InteractionPartner;
}

void AGP3PawnPlayer::SetMovementLock(bool State)
{
	IsMovementLocked = State;
	if(IsMovementLocked)
	{
		MovementComponent->Velocity = FVector::ZeroVector;
		HandleEndSprint();
		HandleEndCrouch();
	}
}

//handles movement and rotation
void AGP3PawnPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!bFinishedLerp)
	{
		LerpCameraPos(DeltaSeconds);
	}
	
	if(IsMovementLocked)
	{
		return;
	}
	FGP3FrameMovement FrameMovement = MovementComponent->CreateFrameMovement();
	bool GroundTest = GroundCheck();
	if(!IsAirborne && !GroundTest)
	{
		IsAirborne = true;
		BecomeAirborne.Broadcast(true);
	}
	else if(IsAirborne && GroundCheck())
	{
		IsAirborne = false;
		BecomeAirborne.Broadcast(false);
	}
	
	FrameMovement.AddGravity(Gravity * DeltaSeconds);
	if (!InputVector.IsNearlyZero())
	{
		FVector Velocity;
		if(!IsPushAndPullLocked)
		{
			RotationDirection = FVector::VectorPlaneProject(Camera->GetForwardVector(), FVector::UpVector);
			FVector Forward = RotationDirection;
			FVector ForwardMovement = Forward * InputVector.X;
			FVector Right = Camera->GetRightVector() * InputVector.Y;
			Velocity = (ForwardMovement + Right).GetSafeNormal() * SprintModifier * CrouchModifier * MovementSpeed;
			CurrentRotationSpeed = MovingRotationSpeed;
		}
		else
		{
			FVector Forward = FVector::VectorPlaneProject(GetActorForwardVector(), FVector::UpVector) * InputVector.X;
			FVector Right = GetActorRightVector() * InputVector.Y;
			Velocity = (Forward + Right).GetSafeNormal() * SprintModifier * CrouchModifier * MovementSpeed * PushAndPullSpeedReduction;
			FHitResult HitResult;
		}
		FrameMovement.AddDelta(Velocity * DeltaSeconds);
		FrameMovement.Velocity = Velocity;	
			
	}
	else
	{
		CurrentRotationSpeed = StationaryRotationSpeed;
	}
	
	MovementComponent->Move(FrameMovement, MovementPartner);
	MovementComponent->SetFacingDirection(RotationDirection, CurrentRotationSpeed);
}

void AGP3PawnPlayer::BeginPlay()
{
	Super::BeginPlay();
	RotationDirection = GetActorForwardVector();
	CurrentRotationSpeed = StationaryRotationSpeed;
	CapsuleRadius = Capsule->GetScaledCapsuleRadius() - 1;
	CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();

	AGP3GameState* GameState = Cast<AGP3GameState>(GetWorld()->GetGameState());

	if(GameState)
	{
		GameState->SetCurrentCheckpoint(GetActorLocation(), GetActorRotation());
	}
}

void AGP3PawnPlayer::HandleMoveForward(float Value)
{	
	InputVector.X = 0;
	
	InputVector.X = Value;
}

void AGP3PawnPlayer::HandleMoveRight(float Value)
{
	InputVector.Y = 0;
	
	InputVector.Y = Value;
}

void AGP3PawnPlayer::HandleLookRight(float Rate)
{
	if(bLockCameraRotation)
	{
		return;	
	}
	
	AddControllerYawInput(0);

	float TurnModifier = GetWorld()->GetDeltaSeconds();

	TurnModifier = FMath::Min(TurnModifier, 0.033f);
	
	AddControllerYawInput(VerticalSensitivity * TurnModifier * Rate);
}

void AGP3PawnPlayer::HandleLookUp(float Rate)
{
	if(bLockCameraRotation)
	{
		return;	
	}

	float TurnModifier = GetWorld()->GetDeltaSeconds();

	TurnModifier = FMath::Min(TurnModifier, 0.016667f);
	
	AddControllerPitchInput(HorizontalSensitivity * TurnModifier * Rate);
}

void AGP3PawnPlayer::HandleStartInteract()
{
	if(IsMovementLocked)
	{
		return;
	}
	if(ConnectedInteractionItem)
	{
		if(!MovementPartner->AllowDropOff())
		{
			return;
		}
		
		ConnectedInteractionItem->StopInteraction(this);
		ConnectedInteractionItem = nullptr;
		MovementPartner = nullptr;
		PerformPush.Broadcast(false);
		return;
	}
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	FHitResult Hit;
	UKismetSystemLibrary::CapsuleTraceSingle(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 50,
		55.f, 96.f, TraceType, false, IgnoreActors, EDrawDebugTrace::None,Hit, true);
	if(Hit.bBlockingHit)
	{
		if(Hit.GetComponent()->GetAttachmentRootActor() == nullptr)
		{
			return;
		}
		UGP3InteractionBase* HitObjectComponent = Cast<UGP3InteractionBase>(Hit.Actor->GetComponentByClass(UGP3InteractionBase::StaticClass()));

		if(HitObjectComponent)
		{
			HitObjectComponent->Interact(this);
		}
		
	}
}

void AGP3PawnPlayer::HandleStartCrouch()
{
	if(!IsPushAndPullLocked && !IsSprinting)
	{
		CrouchModifier = CrouchMultiplier;
		IsCrouching = true;
		Capsule->SetCapsuleHalfHeight(CrouchHalfHeight);
		PerformCrouch.Broadcast(true);
	}
}

void AGP3PawnPlayer::HandleEndCrouch()
{
	CrouchModifier = 1.f;
	IsCrouching = false;
	Capsule->SetCapsuleHalfHeight(96.f);
	PerformCrouch.Broadcast(false);
}

void AGP3PawnPlayer::HandleStartSprint()
{
	if(!IsPushAndPullLocked && !IsCrouching)
	{
		SprintModifier = SprintMultiplier;
		IsSprinting = true;
		PerformSprint.Broadcast(true);
	}
}

void AGP3PawnPlayer::HandleEndSprint()
{
	SprintModifier = 1.f;
	IsSprinting = false;
	PerformSprint.Broadcast(false);
}

void AGP3PawnPlayer::HandleVault()
{
	if(IsPushAndPullLocked || IsMovementLocked)
	{
		return;
	}
	TestVault();
}

void AGP3PawnPlayer::PushAndPullLock(bool LockState)
{
	IsPushAndPullLocked = LockState;
	//PerformPush.Broadcast(IsPushAndPullLocked);
}

void AGP3PawnPlayer::SetPlayerRotation(FRotator NewRotation, bool SnapCamera)
{
	GetOwner()->SetActorRotation(NewRotation);
	MovementComponent->SetForcedRotation(NewRotation);
	RotationDirection = GetOwner()->GetActorForwardVector();
	if(SnapCamera)
	{
		Controller->SetControlRotation(NewRotation);
	}
}

void AGP3PawnPlayer::SetCameraPositionAndRotation(FVector NewLocation, FRotator NewRotation)
{
	Camera->SetRelativeLocation(NewLocation, false);
	Controller->SetControlRotation(NewRotation);
}

void AGP3PawnPlayer::SetCameraPosition(FVector NewLocation)
{
	Camera->SetRelativeLocation(NewLocation, false);
}

void AGP3PawnPlayer::TeleportToCheckpoint()
{
	AGP3GameState* GameState = Cast<AGP3GameState>(GetWorld()->GetGameState());

	if(GameState)
	{
		FVector const NewLocation = GameState->GetCurrentCheckpointPos();
		FRotator const NewRotation = GameState->GetCurrentCheckpointRot();

		SetActorLocation(NewLocation);
		SetPlayerRotation(NewRotation);
	}
}

void AGP3PawnPlayer::SetCameraLock(bool State)
{
	bLockCameraRotation = State;
}

void AGP3PawnPlayer::SetPushState(bool State)
{
	PerformPush.Broadcast(State);
}

void AGP3PawnPlayer::SetMovementLockPush(bool State)
{
	IsMovementLocked = State;
	if(IsMovementLocked)
	{
		HandleEndSprint();
		HandleEndCrouch();
		MovementComponent->Velocity = -GetActorForwardVector() * MovementSpeed;
	}
}

void AGP3PawnPlayer::SetZeroVelocity()
{
	MovementComponent->Velocity = FVector::ZeroVector;
}

void AGP3PawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AGP3PawnPlayer::HandleStartInteract);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AGP3PawnPlayer::HandleStartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AGP3PawnPlayer::HandleEndCrouch);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AGP3PawnPlayer::HandleStartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AGP3PawnPlayer::HandleEndSprint);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGP3PawnPlayer::TestVault);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AGP3PawnPlayer::HandleMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGP3PawnPlayer::HandleMoveRight);
	PlayerInputComponent->BindAxis("LookRight", this, &AGP3PawnPlayer::HandleLookRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AGP3PawnPlayer::HandleLookUp);
}

void AGP3PawnPlayer::TestVault()
{

	if(IsMovementLocked)
	{
		return;
	}
	
	FVector ActorLocation = GetActorLocation();
	FHitResult HitResult;
	TArray<AActor*> IgnoreActors;
	UKismetSystemLibrary::LineTraceSingle(this, ActorLocation, ActorLocation + GetActorForwardVector() * 100, TraceType,
		false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);

	if(HitResult.bBlockingHit)
	{
		FVector HitNormal = HitResult.Normal;
		FVector ForwardHitPoint = HitResult.Location;
		AActor* HitActor = nullptr;
		if(HitResult.GetActor() != nullptr)
		{
			HitActor = HitResult.GetActor();
		}
		HitResult.Reset();
		FVector SecondStart = ForwardHitPoint + FVector::UpVector * 400.f - HitNormal * 25.f;
		FVector SecondEnd = SecondStart - FVector::UpVector * 800.f;

		UKismetSystemLibrary::LineTraceSingle(this, SecondStart, SecondEnd, VaultTrace,
        false, IgnoreActors, EDrawDebugTrace::None, HitResult, true);

		if(HitResult.bBlockingHit)
		{
			FVector DownwardsHitPoint = HitResult.Location;
			if(HitResult.GetActor() != nullptr)
			{
				if(HitActor != HitResult.GetActor())
				{
					return;
				}
			}
			
			if(UKismetMathLibrary::NearlyEqual_FloatFloat(ActorLocation.Z, DownwardsHitPoint.Z, VaultingMaxHeight) && ActorLocation.Z <= DownwardsHitPoint.Z)
			{
				float DotDifference = FVector::DotProduct(FVector::UpVector, HitResult.Normal);
				//UE_LOG(LogTemp, Log, TEXT("Dot product was: %f"), DotDifference);
				if(DotDifference < 0.8f)
				{
					//UE_LOG(LogTemp, Log, TEXT("Wall was too steep"));
					return;
				}
				//UE_LOG(LogTemp, Log, TEXT("wall is climbable"));
				FVector LedgeTop = ForwardHitPoint;
				LedgeTop.Z = DownwardsHitPoint.Z;
				IsMovementLocked = true;
				PerformVault.Broadcast(HitNormal, LedgeTop);
			}
		}
		
	}
}

bool AGP3PawnPlayer::GroundCheck()
{
	TArray<AActor*> IgnoreActors;
	FHitResult GroundHit;
	UKismetSystemLibrary::CapsuleTraceSingle(this, GetActorLocation(), GetActorLocation() - FVector::UpVector * 6, CapsuleRadius, CapsuleHalfHeight,
		TraceType, false, IgnoreActors, EDrawDebugTrace::None, GroundHit, true);
	return GroundHit.bBlockingHit;
}

void AGP3PawnPlayer::LerpCameraBoomToTarget(FTransform TargetTrans)
{
	SetMovementLock(true);
	bLockCameraRotation = true;

	PreviousLocationForLerp = SpringArm->GetComponentLocation();

	if(Controller)
	{
		PreviousRotationForLerp = Controller->GetControlRotation().Quaternion();
	}
	else
	{
		PreviousRotationForLerp = GetActorRotation().Quaternion();
	}

	
	
	TargetLocationForLerp = TargetTrans.GetLocation() - SpringArm->TargetOffset;
	TargetRotationForLerp = TargetTrans.GetRotation();

	CameraLerpAccumulator = 0;
	bFinishedLerp = false;
	bShouldUnlockControl = false;
}

void AGP3PawnPlayer::LerpCameraPos(float DeltaTime)
{
	CameraLerpAccumulator += DeltaTime / CameraLerpTime;

	SpringArm->SetWorldLocation(FMath::Lerp(PreviousLocationForLerp, TargetLocationForLerp, CameraLerpAccumulator));
	SpringArm->SetWorldRotation(FQuat::Slerp(PreviousRotationForLerp, TargetRotationForLerp, CameraLerpAccumulator).Rotator());

	if(Controller)
	{
		Controller->SetControlRotation(FQuat::Slerp(PreviousRotationForLerp, TargetRotationForLerp, CameraLerpAccumulator).Rotator());	
	}

	if(CameraLerpAccumulator >= 1)
	{
		bFinishedLerp = true;
		CameraLerpAccumulator = 0;

		if(bShouldUnlockControl)
		{
			bShouldUnlockControl = false;
			SetMovementLock(false);
			bLockCameraRotation = false;

			SpringArm->SetRelativeLocation(FVector::ZeroVector);
		}
	}
}

void AGP3PawnPlayer::LerpCameraBoomToPlayer()
{
	SetMovementLock(true);
	bLockCameraRotation = true;

	FVector const TargetLoc = PreviousLocationForLerp;
	FQuat const TargetRot = PreviousRotationForLerp;

	PreviousLocationForLerp = TargetLocationForLerp;
	PreviousRotationForLerp = TargetRotationForLerp;

	TargetLocationForLerp = TargetLoc;
	TargetRotationForLerp = TargetRot;

	CameraLerpAccumulator = 0;
	bFinishedLerp = false;
	bShouldUnlockControl = true;
}

UCameraComponent* AGP3PawnPlayer::GetCamera() const
{
	return Camera;
}