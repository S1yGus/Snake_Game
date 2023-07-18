// Snake_Game, all rights reserved.

#include "Framework/SG_Pawn.h"
#include "Camera/CameraComponent.h"

static double HalfFOVTan(double FOV)
{
    return FMath::Tan(FMath::DegreesToRadians(FOV * 0.5));
}

// https://en.wikipedia.org/wiki/Field_of_view_in_video_games
static double VerticalFOV(double HorizontalFOV, double AspectRatioHW)
{
    return 2.0 * FMath::RadiansToDegrees(FMath::Atan(HalfFOVTan(HorizontalFOV) * AspectRatioHW));
}

ASG_Pawn::ASG_Pawn()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    check(Camera);
    Camera->SetRelativeRotation(FRotator{-90.0, 0.0, 0.0});
    Camera->SetupAttachment(GetRootComponent());
}

void ASG_Pawn::UpdateLocation(const Snake::Dim& GridSize, uint32 CellSize, const FTransform& InGridOrigin)
{
    WorldWidth = GridSize.width * CellSize;
    WorldHeight = GridSize.height * CellSize;
    GridOrigin = InGridOrigin;

    if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
    {
        FViewport* Viewport = GEngine->GameViewport->Viewport;
        Viewport->ViewportResizedEvent.AddUObject(this, &ThisClass::OnViewportResized);

#if WITH_EDITOR
        OnViewportResized(Viewport, 0);
#endif
    }
}

void ASG_Pawn::OnViewportResized(FViewport* Viewport, uint32 Value)
{
    if (!Viewport || !Viewport->GetSizeXY().X || !Viewport->GetSizeXY().Y || !WorldHeight)
        return;

    double ZLocation = 0.0;
    const double ViewportAspect = static_cast<double>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const double GridAspect = WorldWidth / WorldHeight;
    if (GridAspect < ViewportAspect)
    {
        ZLocation = WorldHeight / HalfFOVTan(VerticalFOV(Camera->FieldOfView, 1.0 / ViewportAspect));
    }
    else
    {
        ZLocation = WorldWidth / HalfFOVTan(Camera->FieldOfView);
    }

    SetActorLocation(GridOrigin.GetLocation() + 0.5 * FVector{WorldHeight, WorldWidth, ZLocation});
}
