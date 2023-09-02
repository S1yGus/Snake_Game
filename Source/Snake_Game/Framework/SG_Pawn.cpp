// Snake_Game, all rights reserved.

#include "Framework/SG_Pawn.h"
#include "Camera/CameraComponent.h"

using namespace SnakeGame;

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

void ASG_Pawn::UpdateLocation(const Dim& InGridSize, uint32 InCellSize, const FTransform& InGridOrigin)
{
    GridSize = InGridSize;
    CellSize = InCellSize;
    GridOrigin = InGridOrigin;

    if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
    {
        FViewport* Viewport = GEngine->GameViewport->Viewport;
        Viewport->ViewportResizedEvent.Remove(OnViewportResizedHandle);
        Viewport->ViewportResizedEvent.AddUObject(this, &ThisClass::OnViewportResized);

        OnViewportResized(Viewport, 0);
    }
}

void ASG_Pawn::OnViewportResized(FViewport* Viewport, uint32 Value)
{
    const double WorldWidth = GridSize.width * CellSize;
    const double WorldHeight = GridSize.height * CellSize;

    if (!Viewport || !Viewport->GetSizeXY().X || !Viewport->GetSizeXY().Y || !WorldHeight)
        return;

    double ZLocation = 0.0;
    const double ViewportAspect = static_cast<double>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const double GridAspect = WorldWidth / WorldHeight;
    if (GridAspect < ViewportAspect)
    {
        const uint32 MarginHeight = (GridSize.height + GridMargin) * CellSize;
        ZLocation = MarginHeight / HalfFOVTan(VerticalFOV(Camera->FieldOfView, 1.0 / ViewportAspect));
    }
    else
    {
        const uint32 MarginWidth = (GridSize.width + GridMargin) * CellSize;
        ZLocation = MarginWidth / HalfFOVTan(Camera->FieldOfView);
    }

    SetActorLocation(GridOrigin.GetLocation() + 0.5 * FVector{WorldHeight, WorldWidth, ZLocation});
}
