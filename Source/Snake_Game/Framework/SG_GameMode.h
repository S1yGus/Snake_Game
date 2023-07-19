// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Game.h"
#include "SG_GameMode.generated.h"

class ASG_Grid;
class UDataTable;
class AExponentialHeightFog;

UCLASS()
class SNAKE_GAME_API ASG_GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    virtual void StartPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game", Meta = (ClampMin = "10", ClampMax = "100"))
    FUintPoint GridSize{10};

    UPROPERTY(EditDefaultsOnly, Category = "Game", Meta = (ClampMin = "10", ClampMax = "100"))
    uint32 CellSize{10};

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<ASG_Grid> GridVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    UDataTable* SnakeColorsTable;

private:
#pragma region With metadata
    UPROPERTY()
    ASG_Grid* GridView;

    UPROPERTY()
    AExponentialHeightFog* Fog;

    UFUNCTION(Exec, Category = "Console command")
    void NextColor();
#pragma endregion Variables and functions with Unreal Header Tool metadata

    TUniquePtr<SnakeGame::Game> CoreGame;
    uint32 ColorsTableIndex{0};

    void UpdateColors();
    void FindFog();
};
