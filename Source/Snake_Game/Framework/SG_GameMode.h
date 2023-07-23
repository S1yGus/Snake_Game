// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/CoreTypes.h"
#include "Core/Game.h"
#include "SG_GameMode.generated.h"

class ASG_Grid;
class ASG_Snake;
class UDataTable;
class AExponentialHeightFog;

UCLASS()
class SNAKE_GAME_API ASG_GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASG_GameMode();

    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "10", ClampMax = "100"))
    FUintPoint GridSize{10};

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "10", ClampMax = "100"))
    uint32 CellSize{10};

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "2", ClampMax = "10"))
    uint32 SnakeDefaultSize{4};

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0.01", ClampMax = "1.0"))
    float GameSpeed{1.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TSubclassOf<ASG_Grid> GridVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TSubclassOf<ASG_Snake> SnakeVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    UDataTable* SnakeColorsTable;

private:
#pragma region With metadata
    UPROPERTY()
    ASG_Grid* GridView;

    UPROPERTY()
    ASG_Snake* SnakeView;

    UPROPERTY()
    AExponentialHeightFog* Fog;

    UFUNCTION(Exec, Category = "Console command")
    void NextColor();
#pragma endregion Variables and functions with Unreal Header Tool metadata

    TUniquePtr<SnakeGame::Game> CoreGame;
    uint32 ColorsTableIndex{0};
    SnakeGame::Input SnakeInput{SnakeGame::Input::defaultInput};

    void UpdateColors();
    void FindFog();
    SnakeGame::Settings MakeSettings() const;
};
