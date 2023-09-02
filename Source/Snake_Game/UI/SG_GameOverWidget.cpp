// Snake_Game, all rights reserved.

#include "UI/SG_GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USG_GameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(BackToMenuButton);
    BackToMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnBackToMenu);
}

void USG_GameOverWidget::OnBackToMenu()
{
    if (!MenuLevel.IsNull())
    {
        UGameplayStatics::OpenLevel(this, FName{MenuLevel.GetAssetName()});
    }
}
