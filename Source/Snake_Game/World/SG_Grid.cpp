// Snake_Game, all rights reserved.

#include "World/SG_Grid.h"
#include "World/SG_WorldTypes.h"
#include "Core/Grid.h"
#include "Components/LineBatchComponent.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGridView, All, All)

using namespace SnakeGame;

ASG_Grid::ASG_Grid()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    GridMesh = CreateDefaultSubobject<UStaticMeshComponent>("GridMesh");
    check(GridMesh);
    GridMesh->SetupAttachment(GetRootComponent());
}

void ASG_Grid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // DrawDebugGrid();
}

void ASG_Grid::SetModel(const TSharedPtr<Grid>& Grid, uint32 InCellSize)
{
    if (!Grid.IsValid())
    {
        UE_LOG(LogGridView, Fatal, TEXT("Grid model is nullptr, game aborted!"));
    }

    GridSize = Grid->size();
    CellSize = InCellSize;
    WorldWidth = GridSize.width * CellSize;
    WorldHeight = GridSize.height * CellSize;

    check(GridMesh->GetStaticMesh());
    const FBox Box = GridMesh->GetStaticMesh()->GetBoundingBox();
    const FVector BoxSize = Box.GetSize();
    check(BoxSize.X);
    check(BoxSize.Y);
    GridMesh->SetRelativeScale3D(FVector(WorldHeight / BoxSize.X, WorldWidth / BoxSize.Y, 1.0));
    GridMesh->SetRelativeLocation(0.5 * FVector(WorldHeight, WorldWidth, -BoxSize.Z));

    MaterialInstance = GridMesh->CreateAndSetMaterialInstanceDynamic(0);
    if (MaterialInstance)
    {
        MaterialInstance->SetVectorParameterValue(GirdSizeParameterName, FVector(GridSize.height, GridSize.width, 0.0));
    }
}

void ASG_Grid::UpdateColors(const FSnakeDesignTableRow& DesignSet)
{
    if (MaterialInstance)
    {
        MaterialInstance->SetVectorParameterValue(BackgroundColorParameterName, DesignSet.GridBackgroundColor);
        MaterialInstance->SetVectorParameterValue(WallsColorParameterName, DesignSet.GridWallsColor);
        MaterialInstance->SetVectorParameterValue(LinesColorParameterName, DesignSet.GridLinesColor);
    }
}

void ASG_Grid::DrawDebugGrid()
{
    if (!GetWorld() || !GetWorld()->LineBatcher)
        return;

    for (uint32 y = 0; y != GridSize.height + 1; ++y)
    {
        const FVector StartLocation{GetActorLocation() + GetActorForwardVector() * CellSize * y};
        GetWorld()->LineBatcher->DrawLine(StartLocation, StartLocation + GetActorRightVector() * WorldWidth, FLinearColor::Red, 0, 2.0f);
    }

    for (uint32 x = 0; x != GridSize.width + 1; ++x)
    {
        const FVector StartLocation{GetActorLocation() + GetActorRightVector() * CellSize * x};
        GetWorld()->LineBatcher->DrawLine(StartLocation, StartLocation + GetActorForwardVector() * WorldHeight, FLinearColor::Red, 0, 2.0f);
    }
}
