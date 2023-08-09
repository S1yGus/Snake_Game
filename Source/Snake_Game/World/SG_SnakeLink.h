// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "World/SG_BaseCellObject.h"
#include "SG_SnakeLink.generated.h"

UCLASS()
class SNAKE_GAME_API ASG_SnakeLink : public ASG_BaseCellObject
{
    GENERATED_BODY()

public:
    /**
     * Reproduces the teardown effect
     */
    virtual void Teardown() override;
};
