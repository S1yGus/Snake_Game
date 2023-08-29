// Snake_Game, all rights reserved.

#include "UI/SG_BaseGameWidget.h"
#include "Components/TextBlock.h"
#include "World/SG_WorldUtils.h"

void USG_BaseGameWidget::SetScore(uint32 Score)
{
    if (ScoreText)
    {
        ScoreText->SetText(SnakeGame::Utils::FormatScore(Score));
    }
}

void USG_BaseGameWidget::SetResetKeyName(const FString& ResetKeyName)
{
    if (ResetGameText)
    {
        ResetGameText->SetText(FText::FromString(FString::Printf(TEXT("press <%s> to reset"), *ResetKeyName.ToLower())));
    }
}
