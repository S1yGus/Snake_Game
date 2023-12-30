// Snake_Game, all rights reserved.

#include "World/SG_Food.h"
#include "Core/Food.h"
#include "World/SG_WorldUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogFoodView, All, All)

using namespace SnakeGame;

ASG_Food::ASG_Food()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Food::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CoreFood.IsValid())
    {
        SetActorLocation(GridOrigin + Utils::PosToVector(CoreFood.Pin()->position(), GridSize, CellSize));
    }
}

void ASG_Food::SetModel(const TSharedPtr<Food>& Food, uint32 InCellSize, const Dim& InGridSize, const FVector& InGridOrigin)
{
    if (!Food.IsValid())
    {
        UE_LOG(LogFoodView, Fatal, TEXT("Food model is nullptr, game aborted!"));
    }

    CoreFood = Food;
    CellSize = InCellSize;
    GridSize = InGridSize;
    GridOrigin = InGridOrigin;

    SetActorHiddenInGame(false);
}

void ASG_Food::UpdateMesh(UStaticMesh* NewMesh)
{
    if (NewMesh && Mesh->GetStaticMesh() != NewMesh)
    {
        Mesh->SetStaticMesh(NewMesh);
        Mesh->SetMaterial(0, NewMesh->GetMaterial(0));
    }
}
