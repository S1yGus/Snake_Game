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
     * Sets core model to be observed by the food world actor
     * @param Food Model object
     * @param InCellSize World size of the cell
     * @param InGridSize Grid dimensions
     */
    void SetModel(const TSharedPtr<SnakeGame::Food>& Food, uint32 InCellSize, const SnakeGame::Dim& InGridSize, const FVector& InGridOrigin);

    /**
     * Updates mesh and material of the object
     * @param NewMesh Pointer to a new mesh
     */
    void UpdateMesh(UStaticMesh* NewMesh);

    /**
     * Restarts the scale of the object from zero to the target scale
     */
    virtual void RestartScaling() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float PeriodicAmplitudeFactor{0.2f};

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0.0"))
    float PeriodicSpeed{2.0f};

private:
    TWeakPtr<SnakeGame::Food> CoreFood;
    SnakeGame::Dim GridSize;
    uint32 CellSize;
    FVector GridOrigin;
    bool bScalingDone{false};
    float PeriodicTime{0.0f};

    virtual void OnScalingDone() override;
};
