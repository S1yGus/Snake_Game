// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_MenuWidget.generated.h"

class UButton;
class UComboBoxString;
class UHorizontalBox;
class USG_CultureButton;
class USG_GameUserSettings;

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
    TObjectPtr<UButton> ToggleScreenModeButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> SpeedComboBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> SizeComboBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> CultureButtonsBox;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TSoftObjectPtr<UWorld> GameLevel;

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    TSubclassOf<USG_CultureButton> CultureButtonClass;

    virtual void NativeOnInitialized() override;

private:
#pragma region With metadata
    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();

    UFUNCTION()
    void OnToggleScreenMode();

    UFUNCTION()
    void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
#pragma endregion Functions with Unreal Header Tool metadata

    void InitComboBox(TObjectPtr<UComboBoxString> ComboBox, const TArray<FText>& OptionNames, const FText& CurrentOptionName);
    void InitCultureButtonsBox(TObjectPtr<USG_GameUserSettings> GameUserSettings);
};
