// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/CoreTypes.h"
#include "SG_Grid.generated.h"

class UStaticMeshComponent;
struct FSnakeColorsTableRow;

namespace SnakeGame
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

    /**
     * Set core model to be observed by the grid world actor
     * @param Grid Model object
     * @param InCellSize World size of the cell
     */
    void SetModel(const TSharedPtr<SnakeGame::Grid>& Grid, uint32 InCellSize);

    /**
     * Updates grid colors
     * @param ColorsSet The structure with color values
     */
    void UpdateColors(const FSnakeColorsTableRow& ColorsSet);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* GridMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName GirdSizeParameterName{"GridSize"};

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName BackgroundColorParameterName{"BackgroundColor"};

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName WallsColorParameterName{"WallsColor"};

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName LinesColorParameterName{"LinesColor"};

private:
#pragma region With metadata
    UPROPERTY()
    UMaterialInstanceDynamic* MaterialInstance;
#pragma endregion Variables with Unreal Header Tool metadata

    SnakeGame::Dim GridSize;
    uint32 CellSize;
    uint32 WorldWidth;
    uint32 WorldHeight;

    void DrawDebugGrid();
};
