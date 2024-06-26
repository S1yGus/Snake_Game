// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/Utils/TestUtils.h"
#include "Framework/SG_GameMode.h"
#include "Framework/SG_MenuGameMode.h"
#include "Framework/SG_Pawn.h"
#include "Misc/PathViews.h"
#include "Framework/SG_GameUserSettings.h"

using namespace Test;

BEGIN_DEFINE_SPEC(FFramework, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
USG_GameUserSettings* GameUserSettings;

END_DEFINE_SPEC(FFramework)

void FFramework::Define()
{
    Describe("Framework",
             [this]()
             {
                 It("AllLevelsShouldExist",
                    [this]()
                    {
                        TArray<FString> AllLevels;
                        IFileManager::Get().FindFilesRecursive(AllLevels, *FPaths::ProjectContentDir(), *FString{"*.umap"}, true, false);

                        const TArray<FString> SnakeLevels{"GameLevel", "MenuLevel"};
                        for (const auto& SnakeLevel : SnakeLevels)
                        {
                            const auto* FoundLevel = AllLevels.FindByPredicate(
                                [&](const FString& GameLevelPath)
                                {
                                    FStringView OutPath, OutName, OutExt;
                                    FPathViews::Split(GameLevelPath, OutPath, OutName, OutExt);
                                    return OutName.Equals(SnakeLevel);
                                });

                            TestNotNull(FString::Printf(TEXT("%s should exist"), *SnakeLevel), FoundLevel);
                        }
                    });

                 It("GameClassesMustBeSetupCorrectly",
                    [this]()
                    {
                        AutomationOpenMap("/Game/Levels/GameLevel");
                        World = GetTestGameWorld();

                        const auto* PC = World->GetFirstPlayerController();
                        TestNotNull("PlayerController must exist", PC);

                        const auto* Pawn = PC->GetPawn<ASG_Pawn>();
                        TestNotNull("PlayerPawn must be setup correctly", Pawn);

                        const auto* GameMode = World->GetAuthGameMode<ASG_GameMode>();
                        TestNotNull("GameMode must be setup correctly", GameMode);

                        SpecCloseLevel(World);
                    });

                 It("MenuClassesMustBeSetupCorrectly",
                    [this]()
                    {
                        AutomationOpenMap("/Game/Levels/MenuLevel");
                        World = GetTestGameWorld();

                        const auto* GameMode = World->GetAuthGameMode<ASG_MenuGameMode>();
                        TestNotNull("MenuGameMode must be setup correctly", GameMode);

                        SpecCloseLevel(World);
                    });
             });

    Describe("Framework.GameUserSettings",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         GameUserSettings = NewObject<USG_GameUserSettings>();
                     });

                 It("AllSpeedOptionNamesShouldExist",
                    [this]()
                    {
                        const TArray<FString> Payload{"Worm", "Snake"};
                        for (const auto& ExpectedName : Payload)
                        {
                            TestTrueExpr(GameUserSettings->GetSpeedOptionNames().FindByPredicate(
                                             [&ExpectedName](const auto& OptionName)
                                             {
                                                 return OptionName.ToString().Equals(ExpectedName);
                                             }) != nullptr);
                        }
                    });

                 It("SpeedOptionsShouldBeFoundByName",
                    [this]()
                    {
                        const TArray<TestPayload<FString, ESpeedOption>> Payload{{"Worm", ESpeedOption::Worm}, {"Snake", ESpeedOption::Snake}};
                        for (const auto& OnePayload : Payload)
                        {
                            TestTrueExpr(GameUserSettings->GetSpeedOptionByName(OnePayload.TestValue) == OnePayload.ExpectedValue);
                        }
                    });

                 It("SpeedOptionsShouldBeSaved",
                    [this]()
                    {
                        using namespace SnakeGame;
                        const TArray<TestPayload<ESpeedOption, TPair<FString, SpeedData>>> Payload{{ESpeedOption::Worm, {"Worm", SpeedData{0.3f, 0.2f, 0.0f}}},    //
                                                                                                   {ESpeedOption::Snake, {"Snake", SpeedData{0.2f, 0.1f, 0.0f}}}};
                        for (const auto& OnePayload : Payload)
                        {
                            GameUserSettings->SaveSnakeSettings(OnePayload.TestValue, ESizeOption::Size_40x16);
                            TestTrueExpr(GameUserSettings->GetCurrentSpeedOptionName().ToString().Equals(OnePayload.ExpectedValue.Key));
                            TestTrueExpr(GameUserSettings->GetCurrentSpeed() == OnePayload.ExpectedValue.Value);
                        }
                    });

                 It("AllSizeOptionNamesShouldExist",
                    [this]()
                    {
                        const TArray<FString> Payload{"30x12", "40x16", "50x20"};
                        for (const auto& ExpectedName : Payload)
                        {
                            TestTrueExpr(GameUserSettings->GetSizeOptionNames().FindByPredicate(
                                             [&ExpectedName](const auto& OptionName)
                                             {
                                                 return OptionName.ToString().Equals(ExpectedName);
                                             }) != nullptr);
                        }
                    });

                 It("SizeOptionsShouldBeFoundByName",
                    [this]()
                    {
                        const TArray<TestPayload<FString, ESizeOption>> Payload{{"30x12", ESizeOption::Size_30x12},    //
                                                                                {"40x16", ESizeOption::Size_40x16},    //
                                                                                {"50x20", ESizeOption::Size_50x20}};
                        for (const auto& OnePayload : Payload)
                        {
                            TestTrueExpr(GameUserSettings->GetSizeOptionByName(OnePayload.TestValue) == OnePayload.ExpectedValue);
                        }
                    });

                 It("SizeOptionsShouldBeSaved",
                    [this]()
                    {
                        const TArray<TestPayload<ESizeOption, TPair<FString, SnakeGame::Dim>>> Payload{{ESizeOption::Size_30x12, {"30x12", SnakeGame::Dim{30, 12}}},    //
                                                                                                       {ESizeOption::Size_40x16, {"40x16", SnakeGame::Dim{40, 16}}},    //
                                                                                                       {ESizeOption::Size_50x20, {"50x20", SnakeGame::Dim{50, 20}}}};
                        for (const auto& OnePayload : Payload)
                        {
                            GameUserSettings->SaveSnakeSettings(ESpeedOption::Snake, OnePayload.TestValue);
                            TestTrueExpr(GameUserSettings->GetCurrentSizeOptionName().ToString().Equals(OnePayload.ExpectedValue.Key));
                            TestTrueExpr(GameUserSettings->GetCurrentSize() == OnePayload.ExpectedValue.Value);
                        }
                    });
             });
}

#endif
