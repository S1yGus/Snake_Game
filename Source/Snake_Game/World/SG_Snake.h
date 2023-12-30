// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/CoreTypes.h"
#include "SG_Snake.generated.h"

namespace SnakeGame
{
class Snake;
}

class ASG_BaseCellObject;
class ASG_SnakeLink;
class USG_ObjectPool;
struct FSnakeDesignTableRow;

UCLASS()
class SNAKE_GAME_API ASG_Snake : public AActor
{
    GENERATED_BODY()

public:
    ASG_Snake();

    virtual void Tick(float DeltaTime) override;

    /**
     * Sets core model to be observed by the snake world actor
     * @param Snake Model object
     * @param InCellSize World size of the cell
     * @param InGridSize Grid dimensions
     */
    void SetModel(const TSharedPtr<SnakeGame::Snake>& Snake, uint32 InCellSize, const SnakeGame::Dim& InGridSize);

    /**
     * Updates snake colors
     * @param DesignSet The structure with color values
     */
    void UpdateColors(const FSnakeDesignTableRow& DesignSet);

    /**
     * Reproduces the teardown effect for each link and add it to the pool
     */
    void Teardown();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    int32 ReservedLinksNumber{10};

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TSubclassOf<ASG_SnakeLink> SnakeLinkClass;

private:
#pragma region With metadata
    UPROPERTY()
    TArray<TObjectPtr<ASG_BaseCellObject>> Links;
    UPROPERTY()
    TObjectPtr<USG_ObjectPool> LinkPool;
#pragma endregion Variables with Unreal Header Tool metadata

    TWeakPtr<SnakeGame::Snake> CoreSnake;
    uint32 CellSize;
    SnakeGame::Dim GridSize;
    FLinearColor LinkColor;

    void InitPool();
    void EmptyLinks();
    void SpwnLinks();
};
