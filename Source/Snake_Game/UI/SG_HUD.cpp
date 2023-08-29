// Snake_Game, all rights reserved.

#include "UI/SG_HUD.h"
#include "UI/SG_GameWidget.h"
#include "UI/SG_GameOverWidget.h"
#include "Core/Game.h"

DEFINE_LOG_CATEGORY_STATIC(LogSnakeGameHUD, All, All)

using namespace SnakeGame;

static constexpr float TimerRate{1.0f};

void ASG_HUD::SetModel(const TSharedPtr<Game>& Game)
{
    if (!Game.IsValid())
    {
        UE_LOG(LogSnakeGameHUD, Fatal, TEXT("Game model is nullptr, game aborted!"));
    }

    CoreGame = Game;

    SetGameState(EGameState::Game);
    Game->subscribeOnGameEvent(
        [&](GameEvent Event)
        {
            switch (Event)
            {
                case GameEvent::FoodTaken:
                    if (CoreGame.IsValid())
                    {
                        GameWidget->SetScore(CoreGame.Pin()->score());
                    }
                    break;
                case GameEvent::GameCompleted:
                    [[fallthrough]];
                case GameEvent::GameOver:
                    SetGameState(EGameState::GameOver);
                    if (CoreGame.IsValid())
                    {
                        GameOverWidget->SetScore(CoreGame.Pin()->score());
                    }
                    break;
            }
        });
}

void ASG_HUD::SetKeyNames(const FString& ResetKeyName)
{
    GameWidget->SetResetKeyName(ResetKeyName);
    GameOverWidget->SetResetKeyName(ResetKeyName);
}

void ASG_HUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidget = CreateWidget<USG_GameWidget>(GetWorld(), GameWidgetClass);
    check(GameWidget);
    WidgetsMap.Add(EGameState::Game, GameWidget);

    GameOverWidget = CreateWidget<USG_GameOverWidget>(GetWorld(), GameOverWidgetClass);
    check(GameOverWidget);
    WidgetsMap.Add(EGameState::GameOver, GameOverWidget);

    for (auto& [State, Widget] : WidgetsMap)
    {
        Widget->AddToViewport();
        Widget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void ASG_HUD::SetGameState(EGameState GameState)
{
    if (WidgetsMap.Contains(GameState))
    {
        if (CurrentWidget)
        {
            CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
        }

        CurrentWidget = WidgetsMap[GameState];
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentGameState = GameState;

        if (GameState == EGameState::Game && CoreGame.IsValid())
        {
            GameWidget->SetScore(CoreGame.Pin()->score());
            OnTimerUpdate();
            GetWorldTimerManager().SetTimer(Timer, this, &ThisClass::OnTimerUpdate, TimerRate, true);
        }
    }
}

void ASG_HUD::OnTimerUpdate()
{
    if (CoreGame.IsValid() && CurrentGameState == EGameState::Game)
    {
        GameWidget->SetTime(CoreGame.Pin()->gameTime());
    }
}
