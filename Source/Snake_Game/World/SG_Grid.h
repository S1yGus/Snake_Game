// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/CoreTypes.h"
#include "SG_Grid.generated.h"

class UStaticMeshComponent;
class UNiagaraComponent;
struct FSnakeDesignTableRow;

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
     * Sets core model to be observed by the grid world actor
     * @param Grid Model object
     * @param InCellSize World size of the cell
     */
    void SetModel(const TSharedPtr<SnakeGame::Grid>& Grid, uint32 InCellSize);

    /**
     * Updates grid colors
     * @param DesignSet The structure with color values
     */
    void UpdateColors(const FSnakeDesignTableRow& DesignSet);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USceneComponent> Origin;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> GridMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UNiagaraComponent> WallEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName GirdSizeParamName{"GridSize"};

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName BackgroundColorParamName{"BackgroundColor"};

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName WallsColorParamName{"WallsColor"};

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName LinesColorParamName{"LinesColor"};

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    FName WallEffectColorParamName{"Color"};

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    FName WallEffectExternalSizeParamName{"ExternalSize"};

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    FName WallEffectInternalSizeParamName{"InternalSize"};

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    FName WallEffectSpawnRateParamName{"SpawnRate"};

    UPROPERTY(EditDefaultsOnly, Category = "VFX", Meta = (ClampMin = "0.0"))
    float WallEffectSpawnRateFactor{2.4f};

    UPROPERTY(EditDefaultsOnly, Category = "VFX", Meta = (ClampMin = "0.0"))
    float WallThicknessFactor{1.575f};

private:
#pragma region With metadata
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
#pragma endregion Variables with Unreal Header Tool metadata

    SnakeGame::Dim GridSize;
    uint32 CellSize;
    uint32 WorldWidth;
    uint32 WorldHeight;

    FORCEINLINE void SetupGrid();
    FORCEINLINE void SetupWallEffect();
    void DrawDebugGrid();
};
