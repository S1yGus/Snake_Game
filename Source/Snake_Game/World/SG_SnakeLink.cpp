// Snake_Game, all rights reserved.

#include "World/SG_SnakeLink.h"

void ASG_SnakeLink::Teardown()
{
    Super::Teardown();

    SetActorHiddenInGame(true);
}
