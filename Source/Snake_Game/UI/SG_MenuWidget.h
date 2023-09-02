// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_MenuWidget.generated.h"

class UButton;
class UComboBoxString;

UCLASS()
class SNAKE_GAME_API USG_MenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StartGameButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> QuitGameButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> SpeedComboBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> SizeComboBox;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TSoftObjectPtr<UWorld> GameLevel;

    virtual void NativeOnInitialized() override;

private:
#pragma region With metadata
    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();

    UFUNCTION()
    void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
#pragma endregion Functions with Unreal Header Tool metadata

    void InitComboBox(TObjectPtr<UComboBoxString> ComboBox, const TArray<FString>& OptionNames, const FString& CurrentOptionName);
};
