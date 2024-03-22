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

void USG_BaseGameWidget::SetKeyNames(const FString& ResetKeyName, const FString& BackToMenuKeyName)
{
    if (HintText)
    {
        const auto Hint = FText::Format(NSLOCTEXT("Snake_Game_UI", "KeysHint_Loc", "press <{0}> to reset, <{1}> to back to menu"),
                                        FText::FromString(ResetKeyName.ToUpper()), FText::FromString(BackToMenuKeyName.ToUpper()));
        HintText->SetText(Hint);
    }
}
