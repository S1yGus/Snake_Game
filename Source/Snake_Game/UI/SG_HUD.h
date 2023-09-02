// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SG_HUD.generated.h"

class USG_GameWidget;
class USG_GameOverWidget;

UENUM()
enum class EGameState : uint8
{
    Game = 0,
    GameOver,
    GameCompleted
};

namespace SnakeGame
{
class Game;
}

UCLASS()
class SNAKE_GAME_API ASG_HUD : public AHUD
{
    GENERATED_BODY()

public:
    /**
     * Sets core game model to be observed by the HUD
     * @param Game Core game model object
     */
    void SetModel(const TSharedPtr<SnakeGame::Game>& Game);

    /**
     * Sets the names for a control keys
     * @param ResetKeyName Reset key name
     */
    void SetKeyNames(const FString& ResetKeyName);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USG_GameWidget> GameWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USG_GameOverWidget> GameOverWidgetClass;

    virtual void BeginPlay() override;

private:
#pragma region With metadata
    UPROPERTY()
    TObjectPtr<USG_GameWidget> GameWidget;

    UPROPERTY()
    TObjectPtr<USG_GameOverWidget> GameOverWidget;

    UPROPERTY()
    TMap<EGameState, TObjectPtr<UUserWidget>> WidgetsMap;

    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget;
#pragma endregion Variables with Unreal Header Tool metadata

    EGameState CurrentGameState;
    FTimerHandle Timer;
    TWeakPtr<SnakeGame::Game> CoreGame;

    void SetGameState(EGameState GameState);
    void OnTimerUpdate();
};
