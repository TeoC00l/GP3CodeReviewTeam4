#include "GP3Portal.h"

#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GP3_Team4/GP3GameState.h"
#include "GP3_Team4/PlayerCharacter/GP3LocalPlayer.h"
#include "GP3_Team4/PlayerCharacter/GP3PawnPlayer.h"
#include "GP3_Team4/PlayerCharacter/GP3PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"

#include "Engine/PostProcessVolume.h"

AGP3Portal::AGP3Portal()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PostUpdateWork;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent->Mobility = EComponentMobility::Static;

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>("PortalMesh");
	PortalMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PortalMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	PortalMesh->SetupAttachment(RootComponent);
	PortalMesh->CastShadow = false;

	PortalBox = CreateDefaultSubobject<UBoxComponent>("PortalBox");
	PortalBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PortalBox->SetUseCCD(true);
	PortalBox->SetCollisionProfileName("Portal");
	PortalBox->SetupAttachment(PortalMesh);

	PortalCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("PortalCapture");
	PortalCapture->SetupAttachment(RootComponent);
	PortalCapture->bEnableClipPlane = true;
	PortalCapture->bUseCustomProjectionMatrix = false;
	PortalCapture->bCaptureEveryFrame = false;
	PortalCapture->bCaptureOnMovement = false;
	PortalCapture->LODDistanceFactor = 3;
	PortalCapture->TextureTarget = nullptr;
	PortalCapture->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;

	Active = false;
	Initialized = false;
	RecursionAmount = 5;
	ResolutionPercentile = 1.0f;
	OverlapActive = false;
}

void AGP3Portal::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryActorTick.SetTickFunctionEnable(false);
	FTimerHandle Timer;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "Setup");
	GetWorldTimerManager().SetTimer(Timer, TimerDelegate, 1.0f, false, 0.5f);
}

void AGP3Portal::Setup()
{
	pTargetPortal = Cast<AGP3Portal>(TargetPortal);
	if(!TargetPortal || !pTargetPortal)
	{
		UE_LOG(LogTemp, Warning, TEXT("Portal %s was destroyed as there was no target portal or it wasn't a portal class"), *GetName());
		Destroy();
		return;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if(!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player controller could not be found in portal %s"), *GetName());
		Destroy();
		return;
	}
	PortalController = PC;
	
	AGP3PawnPlayer* Player = Cast<AGP3PawnPlayer>(PC->GetPawn());
	if(!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player not be found in portal %s"), *GetName());
		Destroy();
		return;
	}
	PortalCharacter = Player;
	
	CreatePortalTexture();
	if(!RenderTarget || !PortalMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Render target or portal material could not be found in portal %s"), *GetName());
		Destroy();
		return;
	}

	Initialized = true;

	LastCharacterLoc = PortalCharacter->Camera->GetComponentLocation();
	
	PrimaryActorTick.SetTickFunctionEnable(true);
}

void AGP3Portal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Initialized)
	{
		SetScalarParameter(0);
		ClearPortalView();

		if(Active || OverlapActive)
		{
			UpdatePortalView();
			UpdateWorldOffset();
			UpdatePawnTracking();
		}
	}
}

bool AGP3Portal::IsActive() const
{
	return Active;
}

void AGP3Portal::SetActive(bool const NewActive)
{
	Active = NewActive;
}

void AGP3Portal::HideActor(AActor* Actor, bool const Hide)
{
	if(Actor->IsValidLowLevel())
	{
		TArray<UActorComponent*> Components;
		Actor->GetComponents(Components);
		for(UActorComponent* Comp : Components)
		{
			UStaticMeshComponent* StaticComp = Cast<UStaticMeshComponent>(Comp);

			if(StaticComp)
			{
				StaticComp->SetRenderInMainPass(!Hide);
			}
		}
	}
}

void AGP3Portal::CreatePortalTexture()
{
	int32 ViewportX, ViewportY;
	PortalController->GetViewportSize(ViewportX, ViewportY);
	ViewportX *= ResolutionPercentile;
	ViewportY *= ResolutionPercentile;

	RenderTarget = nullptr;
	RenderTarget = UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(GetWorld(), UCanvasRenderTarget2D::StaticClass(), ViewportX, ViewportY);

	PortalMaterial = PortalMesh->CreateDynamicMaterialInstance(0, PortalMaterialInstance);
	PortalMaterial->SetTextureParameterValue("RT_Portal", RenderTarget);

	PortalCapture->TextureTarget = RenderTarget;
}

void AGP3Portal::ClearPortalView() const
{
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), RenderTarget);
}

void AGP3Portal::UpdatePortalView()
{
	//PortalCapture->PostProcessSettings = PortalCharacter->Camera->PostProcessSettings;

	if(pTargetPortal->PostProcessVolume)
	{
		//PortalCapture->PostProcessSettings = pTargetPortal->PostProcessVolume->Settings;
	}
	
	PortalCapture->bEnableClipPlane = true;
	PortalCapture->bOverride_CustomNearClippingPlane = true;
	PortalCapture->ClipPlaneNormal = pTargetPortal->PortalMesh->GetForwardVector();
	PortalCapture->ClipPlaneBase = pTargetPortal->PortalMesh->GetComponentLocation() - PortalCapture->ClipPlaneNormal;

	UGP3LocalPlayer* LocalPlayer = Cast<UGP3LocalPlayer>(PortalController->GetLocalPlayer());
	if(!LocalPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("Could find Local player"));
		Destroy();
		return;
	}
	PortalCapture->bUseCustomProjectionMatrix = true;
	PortalCapture->CustomProjectionMatrix = LocalPlayer->GetCameraProjectionMatrix();

	UCameraComponent* PlayerCamera = PortalCharacter->Camera;
	FVector const NewCameraLocation = ConvertLocationToPortal(PlayerCamera->GetComponentLocation(), this, pTargetPortal);
	FRotator const NewCameraRotation = ConvertRotationToPortal(PlayerCamera->GetComponentRotation(), this, pTargetPortal);

	for(int i = RecursionAmount; i >= 0; i--)
	{
		FVector RecursiveCameraLocation = NewCameraLocation;
		FRotator RecursiveCameraRotator = NewCameraRotation;
		for(int j = 0; j < i; j++)
		{
			RecursiveCameraLocation = ConvertLocationToPortal(RecursiveCameraLocation, this, pTargetPortal);
			RecursiveCameraRotator = ConvertRotationToPortal(RecursiveCameraRotator, this, pTargetPortal);
		}
		PortalCapture->SetWorldLocationAndRotation(RecursiveCameraLocation, RecursiveCameraRotator);

		if(i == RecursionAmount)
		{
			PortalMesh->SetVisibility(false);
		}

		PortalCapture->CaptureScene();

		if(i == RecursionAmount)
		{
			PortalMesh->SetVisibility(true);
		}
	}
}

void AGP3Portal::UpdateWorldOffset()
{
	if(!IsInFront(PortalCharacter->Camera->GetComponentLocation()))
	{
		SetScalarParameter(0);
		return;
	}
	
	float const Distance = FVector::Distance(PortalCharacter->Camera->GetComponentLocation(), GetActorLocation());
	float const InvLerp = InverseLerp(500, 200, Distance);

	float DotCamera = FVector::DotProduct(PortalCharacter->Camera->GetForwardVector(), PortalMesh->GetForwardVector());

	FVector const Direction = (PortalCharacter->GetActorLocation() - PortalMesh->GetComponentLocation()).GetSafeNormal();
	float PlayerDot = FVector::DotProduct(Direction, PortalMesh->GetForwardVector());
	
	if(DotCamera < 0)
	{
		DotCamera *= -1;
	}

	if(PlayerDot < 0)
	{
		PlayerDot *= -1;
	}

	DotCamera = FMath::Clamp(DotCamera, 0.2f, 1.0f);
	PlayerDot = FMath::Clamp(PlayerDot, 0.2f, 1.0f);
	
	SetScalarParameter(InvLerp * DotCamera * PlayerDot);
}

void AGP3Portal::UpdatePawnTracking()
{
	FVector const CurrentLocation = PortalCharacter->Camera->GetComponentLocation();

	if(CurrentLocation.ContainsNaN())
	{
		return;
	}

	FVector PointIntersection;
	FPlane const PortalPlane = FPlane(PortalMesh->GetComponentLocation(), PortalMesh->GetForwardVector());

	bool const PassedThoughPlane = FMath::SegmentPlaneIntersection(LastCharacterLoc, CurrentLocation, PortalPlane, PointIntersection);
	FVector const RelativeIntersection = PortalMesh->GetComponentTransform().InverseTransformPositionNoScale(PointIntersection);
	FVector const PortalSize = PortalBox->GetScaledBoxExtent();
	bool const PassedWithinPortal = FMath::Abs(RelativeIntersection.Z) <= PortalSize.Z && FMath::Abs(RelativeIntersection.Y) <= PortalSize.Y;

	if(PassedThoughPlane && PassedWithinPortal && IsInFront(LastCharacterLoc))
	{
		TeleportPlayer();
	}

	LastCharacterLoc = PortalCharacter->Camera->GetComponentLocation();
}

void AGP3Portal::TeleportPlayer()
{
	if(!PortalCharacter)
	{
		return;
	}

	UGP3LocalPlayer* LocalPlayer = Cast<UGP3LocalPlayer>(PortalController->GetLocalPlayer());
	if(!LocalPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("Could find Local player"));
		Destroy();
		return;
	}
	
	SetScalarParameter(0);
	LocalPlayer->CameraCut();

	FVector const ConvertedLocation = ConvertLocationToPortal(PortalCharacter->GetActorLocation(), this, pTargetPortal);
	FRotator const ConvertedRotation = ConvertRotationToPortal(PortalCharacter->GetActorRotation(), this, pTargetPortal);
	FVector const CameraLocalLocation = PortalCharacter->Camera->GetRelativeLocation();
	FRotator const CameraRotation = PortalCharacter->GetControlRotation();
	//FRotator const CameraRotation = ConvertRotationToPortal(PortalCharacter->GetControlRotation(), this, pTargetPortal);;
	
	// Set Player Rotation and location
	PortalCharacter->SetActorLocationAndRotation(ConvertedLocation, ConvertedRotation);
	
	// Camera Position and Rotation
	PortalCharacter->SetCameraPositionAndRotation(CameraLocalLocation, CameraRotation);
	PortalCharacter->SetPlayerRotation(ConvertedRotation, true);
	
	pTargetPortal->UpdateWorldOffset();
	pTargetPortal->UpdatePortalView();
	pTargetPortal->LastCharacterLoc = PortalCharacter->Camera->GetComponentLocation();
}

bool AGP3Portal::IsInFront(FVector const Location) const
{
	FVector const Direction = (Location - GetActorLocation()).GetSafeNormal();
	float const DotProduct = FVector::DotProduct(Direction, GetActorForwardVector());
	
	return DotProduct >= 0;
}

FVector AGP3Portal::ConvertLocationToPortal(FVector const Location, AGP3Portal* CurrentPortal, AGP3Portal* EndPortal, bool const Flip)
{
	FVector PosRelativeToPortal = CurrentPortal->PortalMesh->GetComponentTransform().InverseTransformPositionNoScale(Location);

	if(Flip)
	{
		PosRelativeToPortal.X *= -1;
		PosRelativeToPortal.Y *= -1;
	}
	FVector const NewWorldLocation = EndPortal->PortalMesh->GetComponentTransform().TransformPositionNoScale(PosRelativeToPortal);

	return NewWorldLocation;
}

FRotator AGP3Portal::ConvertRotationToPortal(FRotator Rotation, AGP3Portal* CurrentPortal, AGP3Portal* EndPortal, bool Flip)
{
	FRotator RelativeRotation = CurrentPortal->PortalMesh->GetComponentTransform().InverseTransformRotation(Rotation.Quaternion()).Rotator();

	if(Flip)
	{
		RelativeRotation.Yaw += 180.0f;
	}
	FRotator const NewWorldRotation = EndPortal->PortalMesh->GetComponentTransform().TransformRotation(RelativeRotation.Quaternion()).Rotator();

	return NewWorldRotation;
}

bool AGP3Portal::LocationInsidePortal(FVector const Location) const
{
	FVector const HalfHeight = PortalBox->GetScaledBoxExtent();
	FVector const Direction = Location - PortalBox->GetComponentLocation();

	bool const WithinX = FMath::Abs(FVector::DotProduct(Direction, PortalBox->GetForwardVector())) <= HalfHeight.X;
	bool const WithinY = FMath::Abs(FVector::DotProduct(Direction, PortalBox->GetRightVector())) <= HalfHeight.Y;
	bool const WithinZ = FMath::Abs(FVector::DotProduct(Direction, PortalBox->GetUpVector())) <= HalfHeight.Z;

	return WithinX && WithinY && WithinZ;
}

void AGP3Portal::SetOverlapActive(bool const NewActive)
{
	OverlapActive = NewActive;
}

float AGP3Portal::InverseLerp(float const A, float const B, float const Distance)
{
	return FMath::Clamp((Distance - A) / (B - A), 0.0f, 1.0f);
}

void AGP3Portal::SetScalarParameter_Implementation(float Value)
{
}
