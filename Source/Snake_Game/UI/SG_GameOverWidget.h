// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SG_BaseGameWidget.h"
#include "SG_GameOverWidget.generated.h"

class UButton;

UCLASS()
class SNAKE_GAME_API USG_GameOverWidget : public USG_BaseGameWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ResetButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> BackToMenuButton;

    virtual void NativeOnInitialized() override;

private:
#pragma region With metadata
    UFUNCTION()
    void OnReset();
    UFUNCTION()
    void OnBackToMenu();
#pragma endregion Functions with Unreal Header Tool metadata
};
