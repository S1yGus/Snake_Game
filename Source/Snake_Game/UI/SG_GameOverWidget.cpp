// Snake_Game, all rights reserved.

#include "UI/SG_GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/SG_GameMode.h"

void USG_GameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ResetButton);
    ResetButton->OnClicked.AddDynamic(this, &ThisClass::OnReset);
    check(BackToMenuButton);
    BackToMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnBackToMenu);
}

void USG_GameOverWidget::OnReset()
{
    if (GetWorld())
    {
        if (auto* GameMode = GetWorld()->GetAuthGameMode<ASG_GameMode>())
        {
            GameMode->Reset();
        }
    }
}

void USG_GameOverWidget::OnBackToMenu()
{
    if (GetWorld())
    {
        if (auto* GameMode = GetWorld()->GetAuthGameMode<ASG_GameMode>())
        {
            GameMode->BackToMenu();
        }
    }
}
