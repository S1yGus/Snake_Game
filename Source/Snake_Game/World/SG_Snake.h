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

class ASG_SnakeLink;
struct FSnakeColorsTableRow;

UCLASS()
class SNAKE_GAME_API ASG_Snake : public AActor
{
    GENERATED_BODY()

public:
    ASG_Snake();

    virtual void Tick(float DeltaTime) override;

    /**
     * Set core model to be observed by the snake world actor
     * @param Snake Model object
     * @param InCellSize World size of the cell
     * @param InGridSize Grid dimensions
     */
    void SetModel(const TSharedPtr<SnakeGame::Snake>& Snake, uint32 InCellSize, const SnakeGame::Dim& InGridSize);

    /**
     * Updates snake colors
     * @param ColorsSet The structure with color values
     */
    void UpdateColors(const FSnakeColorsTableRow& ColorsSet);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TSubclassOf<ASG_SnakeLink> SnakeHeadClass;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TSubclassOf<ASG_SnakeLink> SnakeBodyClass;

private:
#pragma region With metadata
    UPROPERTY()
    TArray<ASG_SnakeLink*> Links;
#pragma endregion Variables with Unreal Header Tool metadata

    TWeakPtr<SnakeGame::Snake> CoreSnake;
    uint32 CellSize;
    SnakeGame::Dim GridSize;
    FLinearColor LinkColor;

    void EmptyLinks();
    void SpwnLinks();
};
