// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/CoreTypes.h"
#include "Core/Game.h"
#include "InputActionValue.h"
#include "SG_GameMode.generated.h"

class ASG_Grid;
class ASG_Snake;
class ASG_Food;
class UDataTable;
class AExponentialHeightFog;
class UInputAction;
class UInputMappingContext;
class ASG_HUD;
struct FFruitData;

UCLASS()
class SNAKE_GAME_API ASG_GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASG_GameMode();

    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    /**
     * Reset current game
     */
    void Reset();

    /**
     * Back to main menu
     */
    void BackToMenu();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TSoftObjectPtr<UWorld> MenuLevel;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    bool bOverrideUserSettings{false};

    UPROPERTY(EditDefaultsOnly, Category = "Settings|GameSpeed", Meta = (ClampMin = "0.0", EditCondition = "bOverrideUserSettings"))
    float InitialSpeed{1.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Settings|GameSpeed", Meta = (ClampMin = "0.0", EditCondition = "bOverrideUserSettings"))
    float SpeedLimit{0.5f};

    UPROPERTY(EditDefaultsOnly, Category = "Settings|GameSpeed", Meta = (ClampMin = "0.0", EditCondition = "bOverrideUserSettings"))
    float Boost{0.1f};

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "10", ClampMax = "100", EditCondition = "bOverrideUserSettings"))
    FUintPoint GridSize{10};

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "10", ClampMax = "100"))
    uint32 CellSize{10};

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "2", ClampMax = "10"))
    uint32 SnakeDefaultSize{4};

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    TSubclassOf<ASG_Grid> GridVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    TSubclassOf<ASG_Snake> SnakeVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    TSubclassOf<ASG_Food> FoodVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Design")
    TObjectPtr<UDataTable> SnakeDesignTable;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputAction> MoveForwardInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputAction> MoveRightInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputAction> ResetInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputAction> BackToMenuInputAction;

    UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
    TObjectPtr<UInputMappingContext> SnakeInputMapping;

private:
#pragma region With metadata
    UPROPERTY()
    TObjectPtr<ASG_Grid> GridView;

    UPROPERTY()
    TObjectPtr<ASG_Snake> SnakeView;

    UPROPERTY()
    TObjectPtr<ASG_Food> FoodView;

    UPROPERTY()
    TObjectPtr<AExponentialHeightFog> Fog;

    UPROPERTY()
    TArray<FFruitData> FruitData;

    UPROPERTY()
    TObjectPtr<ASG_HUD> HUD;

    UFUNCTION(Exec, Category = "Console command")
    void NextDesign();
#pragma endregion Variables and functions with Unreal Header Tool metadata

    TSharedPtr<SnakeGame::Game> CoreGame;
    uint32 DesignTableIndex{0};
    SnakeGame::Input SnakeInput{SnakeGame::Input::defaultInput};

    void UpdateDesign();
    void FindFog();
    FORCEINLINE void RandomizeFoodMesh();

    SnakeGame::Settings MakeSettings() const;

    void SetupInput();
    void OnMoveForward(const FInputActionValue& Value);
    void OnMoveRight(const FInputActionValue& Value);

    void SubscribeOnGameEvent();
};
