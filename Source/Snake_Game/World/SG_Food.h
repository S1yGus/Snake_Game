// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "World/SG_BaseCellObject.h"
#include "Core/CoreTypes.h"
#include "SG_Food.generated.h"

namespace SnakeGame
{
class Food;
}

UCLASS()
class SNAKE_GAME_API ASG_Food : public ASG_BaseCellObject
{
    GENERATED_BODY()

public:
    ASG_Food();

    virtual void Tick(float DeltaTime) override;

    /**
     * Set core model to be observed by the food world actor
     * @param Food Model object
     * @param InCellSize World size of the cell
     * @param InGridSize Grid dimensions
     */
    void SetModel(const TSharedPtr<SnakeGame::Food>& Food, uint32 InCellSize, const SnakeGame::Dim& InGridSize, const FVector& InGridOrigin);

private:
    TWeakPtr<SnakeGame::Food> CoreFood;
    SnakeGame::Dim GridSize;
    uint32 CellSize;
    FVector GridOrigin;
};
