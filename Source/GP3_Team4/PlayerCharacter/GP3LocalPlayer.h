// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "GP3LocalPlayer.generated.h"

UCLASS()
class GP3_TEAM4_API UGP3LocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:
	UGP3LocalPlayer();
	virtual FSceneView* CalcSceneView(FSceneViewFamily* ViewFamily, FVector& OutViewLocation, FRotator& OutViewRotation, FViewport* Viewport, FViewElementDrawer* ViewDrawer, EStereoscopicPass StereoPass) override;

	void CameraCut();

	FMatrix GetCameraProjectionMatrix();
	
private:
	bool bCamCut;
};
