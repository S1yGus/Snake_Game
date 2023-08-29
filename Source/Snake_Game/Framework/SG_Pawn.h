// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/CoreTypes.h"
#include "SG_Pawn.generated.h"

class UCameraComponent;

UCLASS()
class SNAKE_GAME_API ASG_Pawn : public APawn
{
    GENERATED_BODY()

public:
    ASG_Pawn();

    /**
     * Updates pawn height fitting grid in viewport
     * @param InGridSize Grid dimensions
     * @param InCellSize World size of the cell
     * @param InGridOrigin World transformation of the grid
     */
    void UpdateLocation(const SnakeGame::Dim& InGridSize, uint32 InCellSize, const FTransform& InGridOrigin);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USceneComponent> Origin;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(VisibleAnywhere, Category = "Design", Meta = (ClampMin = "0"))
    int32 GridMargin{2};

private:
    SnakeGame::Dim GridSize;
    uint32 CellSize;
    FTransform GridOrigin;
    FDelegateHandle OnViewportResizedHandle;

    void OnViewportResized(FViewport* Viewport, uint32 Value);
};
