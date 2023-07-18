// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/CoreTypes.h"
#include "SG_Grid.generated.h"

namespace Snake
{

class Grid;

}

UCLASS()
class SNAKE_GAME_API ASG_Grid : public AActor
{
    GENERATED_BODY()

public:
    ASG_Grid();

    virtual void Tick(float DeltaTime) override;

    void InitModel(const TSharedPtr<Snake::Grid>& Grid, uint32 InCellSize);

protected:
    virtual void BeginPlay() override;

private:
    Snake::Dim GridSize;
    uint32 CellSize;
    uint32 WorldWidth;
    uint32 WorldHeight;

    void DrawGrid();
};
