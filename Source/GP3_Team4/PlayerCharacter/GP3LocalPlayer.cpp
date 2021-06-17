#include "GP3LocalPlayer.h"

UGP3LocalPlayer::UGP3LocalPlayer()
{
	bCamCut = false;
}

FSceneView* UGP3LocalPlayer::CalcSceneView(FSceneViewFamily* ViewFamily, FVector& OutViewLocation, FRotator& OutViewRotation, FViewport* Viewport, FViewElementDrawer* ViewDrawer, EStereoscopicPass StereoPass)
{
	FSceneView* View = Super::CalcSceneView(ViewFamily, OutViewLocation, OutViewRotation, Viewport, ViewDrawer, StereoPass);
	if(bCamCut)
	{
		View->bCameraCut = true;
		bCamCut = false;
	}

	return View;
}

void UGP3LocalPlayer::CameraCut()
{
	bCamCut = true;
}

FMatrix UGP3LocalPlayer::GetCameraProjectionMatrix()
{
	FSceneViewProjectionData ProjectionData;
	GetProjectionData(ViewportClient->Viewport, EStereoscopicPass::eSSP_FULL, ProjectionData);
	FMatrix const ProjectionMatrix = ProjectionData.ProjectionMatrix;
	return ProjectionMatrix;
}
