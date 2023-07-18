// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Game.h"
#include "SG_GameMode.generated.h"

class ASG_Grid;

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

private:
    TUniquePtr<Snake::Game> CoreGame;

    UPROPERTY()
    ASG_Grid* GridView;
};
