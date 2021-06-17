#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP3Portal.generated.h"

UCLASS()
class GP3_TEAM4_API AGP3Portal : public AActor
{
	GENERATED_BODY()

public:
	AGP3Portal();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void Setup();
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Portal)
	class UStaticMeshComponent* PortalMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Portal)
	class UBoxComponent* PortalBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Portal)
	class USceneCaptureComponent2D* PortalCapture;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Portal)
	class AActor* TargetPortal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Portal)
	class UMaterialInterface* PortalMaterialInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Portal)
	int RecursionAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Portal)
	float ResolutionPercentile;
	
	UPROPERTY(Transient)
	AGP3Portal* pTargetPortal;

	UPROPERTY(Transient)
	bool Active;

	UFUNCTION(BlueprintCallable, Category=Portal)
	bool IsActive() const;

	UFUNCTION(BlueprintCallable, Category=Portal)
	void SetActive(bool const NewActive);

	UFUNCTION()
	static void HideActor(AActor* Actor, bool const Hide = true);

	UFUNCTION(BlueprintCallable, Category=Portal)
	void UpdatePortalView();

	UFUNCTION(BlueprintCallable, Category=Portal)
	void ClearPortalView() const;

	void UpdateWorldOffset();

	UFUNCTION(BlueprintCallable, Category=Portal)
	bool IsInFront(FVector const Location) const;

	UFUNCTION(BlueprintCallable, Category=Portal)
	static FVector ConvertLocationToPortal(FVector const Location, AGP3Portal* CurrentPortal, AGP3Portal* EndPortal, bool const Flip = true);
	
	UFUNCTION(BlueprintCallable, Category=Portal)
	static FRotator ConvertRotationToPortal(FRotator Rotation, AGP3Portal* CurrentPortal, AGP3Portal* EndPortal, bool Flip = true);

	UFUNCTION(BlueprintCallable, Category=Portal)
	bool LocationInsidePortal(FVector const Location) const;

	UFUNCTION(BlueprintCallable, Category=Portal)
	void SetOverlapActive(bool const NewActive);

	UFUNCTION(BlueprintNativeEvent)
	void SetScalarParameter(float Value);

	UFUNCTION()
	static float InverseLerp(float const A, float const B, float const Distance);

	UPROPERTY(EditInstanceOnly, Category=Test)
	class APostProcessVolume* PostProcessVolume;
	
protected:
	UPROPERTY()
	class APlayerController* PortalController;
	
	UPROPERTY()
	class AGP3PawnPlayer* PortalCharacter;
	
	UPROPERTY()
	class UCanvasRenderTarget2D* RenderTarget;

	UPROPERTY()
	TArray<UCanvasRenderTarget2D*> RenderTargets;

	UPROPERTY()
	class UMaterialInstanceDynamic* PortalMaterial;
	
private:
	bool Initialized;
	FVector LastCharacterLoc;
	bool OverlapActive;

	void TeleportPlayer();
	void CreatePortalTexture();
	void UpdatePawnTracking();
};
