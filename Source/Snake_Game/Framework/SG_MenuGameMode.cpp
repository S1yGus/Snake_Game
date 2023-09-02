// Snake_Game, all rights reserved.

#include "Framework/SG_MenuGameMode.h"
#include "UI/SG_MenuHUD.h"
#include "World/SG_WorldUtils.h"

ASG_MenuGameMode::ASG_MenuGameMode()
{
    HUDClass = ASG_MenuHUD::StaticClass();
}

void ASG_MenuGameMode::StartPlay()
{
    Super::StartPlay();

    SnakeGame::Utils::SetUIInput(GetWorld(), true);
}
