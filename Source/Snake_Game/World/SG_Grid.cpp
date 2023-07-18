// Snake_Game, all rights reserved.

#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "Components/LineBatchComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGridView, All, All)

using namespace Snake;

ASG_Grid::ASG_Grid()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Grid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawGrid();
}

void ASG_Grid::InitModel(const TSharedPtr<Grid>& Grid, uint32 InCellSize)
{
    if (!Grid.IsValid())
    {
        UE_LOG(LogGridView, Fatal, TEXT("Grid is nullptr, game aborted!"));
    }

    GridSize = Grid->size();
    CellSize = InCellSize;
    WorldWidth = GridSize.width * CellSize;
    WorldHeight = GridSize.height * CellSize;
}

void ASG_Grid::BeginPlay()
{
    Super::BeginPlay();
}

void ASG_Grid::DrawGrid()
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
