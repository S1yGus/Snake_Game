// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SG_MenuGameMode.generated.h"

UCLASS()
class SNAKE_GAME_API ASG_MenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASG_MenuGameMode();

    virtual void StartPlay() override;
};
