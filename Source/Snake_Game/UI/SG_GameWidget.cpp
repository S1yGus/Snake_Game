// Snake_Game, all rights reserved.

#include "UI/SG_GameWidget.h"
#include "Components/TextBlock.h"
#include "World/SG_WorldUtils.h"

void USG_GameWidget::SetTime(float Seconds)
{
    if (TimeText)
    {
        TimeText->SetText(SnakeGame::Utils::FormatTime(Seconds));
    }
}
