// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/Utils/TestUtils.h"
#include "UI/SG_MenuWidget.h"
#include "UI/SG_GameWidget.h"
#include "UI/SG_GameOverWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Framework/SG_GameUserSettings.h"

using namespace Test;

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FGameOverWidgetVisibleLatentCommand, USG_GameOverWidget*, GameOverWidget);
bool FGameOverWidgetVisibleLatentCommand::Update()
{
    return GameOverWidget->GetVisibility() == ESlateVisibility::Visible;
}

BEGIN_DEFINE_SPEC(FSnakeUI, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;

END_DEFINE_SPEC(FSnakeUI)

void FSnakeUI::Define()
{
    Describe("UI",
             [this]()
             {
                 It("AllMenuComponentsShouldExist",
                    [this]()
                    {
                        AutomationOpenMap("/Game/Levels/MenuLevel");
                        World = GetTestGameWorld();

                        const auto* MenuWidget = FindWidgetByClass<USG_MenuWidget>();
                        if (!TestNotNull("Menu widget should exist", MenuWidget))
                            return;

                        const auto* GameUserSettings = USG_GameUserSettings::Get();
                        if (!TestNotNull("GameUserSettings should exist", GameUserSettings))
                            return;

                        const auto* CultureButtonsBox = Cast<UHorizontalBox>(FindWidgetByName(MenuWidget, "CultureButtonsBox"));
                        if (!TestNotNull("CultureButtonsBox widget should exist", CultureButtonsBox))
                            return;

                        TestTrueExpr(CultureButtonsBox->GetAllChildren().Num() == GameUserSettings->GetCultureOptions().Num());

                        const auto* StartGameButton = Cast<UButton>(FindWidgetByName(MenuWidget, "StartGameButton"));
                        TestTrueExpr(StartGameButton != nullptr);

                        const auto* QuitGameButton = Cast<UButton>(FindWidgetByName(MenuWidget, "QuitGameButton"));
                        TestTrueExpr(QuitGameButton != nullptr);

                        const auto* SpeedComboBox = Cast<UComboBoxString>(FindWidgetByName(MenuWidget, "SpeedComboBox"));
                        TestTrueExpr(SpeedComboBox != nullptr);

                        const auto* SizeComboBox = Cast<UComboBoxString>(FindWidgetByName(MenuWidget, "SizeComboBox"));
                        TestTrueExpr(SizeComboBox != nullptr);

                        SpecCloseLevel(World);
                    });

                 It("AllGameComponentsShouldExist",
                    [this]()
                    {
                        AutomationOpenMap("/Game/Levels/GameLevel");
                        World = GetTestGameWorld();

                        const auto* GameWidget = FindWidgetByClass<USG_GameWidget>();
                        if (!TestNotNull("Game widget should exist", GameWidget))
                            return;

                        const auto* TimeText = Cast<UTextBlock>(FindWidgetByName(GameWidget, "TimeText"));
                        TestTrueExpr(TimeText != nullptr);

                        const auto* ScoreText = Cast<UTextBlock>(FindWidgetByName(GameWidget, "ScoreText"));
                        TestTrueExpr(ScoreText != nullptr);

                        const auto* HintText = Cast<UTextBlock>(FindWidgetByName(GameWidget, "HintText"));
                        TestTrueExpr(HintText != nullptr);

                        SpecCloseLevel(World);
                    });

                 It("AllGameOverComponentsShouldExist",
                    [this]()
                    {
                        AutomationOpenMap("/Game/Levels/GameLevel");
                        World = GetTestGameWorld();

                        auto* GameOverWidget = FindWidgetByClass<USG_GameOverWidget>();
                        if (!TestNotNull("Game over widget should exist", GameOverWidget))
                            return;

                        TestTrueExpr(GameOverWidget->GetVisibility() == ESlateVisibility::Collapsed);

                        ADD_LATENT_AUTOMATION_COMMAND(FGameOverWidgetVisibleLatentCommand(GameOverWidget));
                        ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                            [this, GameOverWidget]()
                            {
                                const auto* ScoreText = Cast<UTextBlock>(FindWidgetByName(GameOverWidget, "ScoreText"));
                                TestTrueExpr(ScoreText != nullptr);

                                const auto* HintText = Cast<UTextBlock>(FindWidgetByName(GameOverWidget, "HintText"));
                                TestTrueExpr(HintText != nullptr);

                                const auto* ResetButton = Cast<UButton>(FindWidgetByName(GameOverWidget, "ResetButton"));
                                TestTrueExpr(ResetButton != nullptr);

                                const auto* BackToMenuButton = Cast<UButton>(FindWidgetByName(GameOverWidget, "BackToMenuButton"));
                                TestTrueExpr(BackToMenuButton != nullptr);

                                return true;
                            }));
                    });
             });
}

#endif
