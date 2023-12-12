// Snake_Game, all rights reserved.

#include "UI/SG_MenuWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Framework/SG_GameUserSettings.h"

void USG_MenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(StartGameButton);
    StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGame);
    check(QuitGameButton);
    QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitGame);

    if (const auto* GameUserSettings = USG_GameUserSettings::Get())
    {
        InitComboBox(SpeedComboBox, GameUserSettings->GetSpeedOptionNames(), GameUserSettings->GetCurrentSpeedOptionName());
        InitComboBox(SizeComboBox, GameUserSettings->GetSizeOptionNames(), GameUserSettings->GetCurrentSizeOptionName());
    }
}

void USG_MenuWidget::OnStartGame()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(this, GameLevel);
}

void USG_MenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Type::Quit, false);
}

void USG_MenuWidget::OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (auto* GameUserSettings = USG_GameUserSettings::Get(); GameUserSettings && SelectionType != ESelectInfo::Type::Direct)
    {
        const TOptional<ESpeedOption> SpeedOption = GameUserSettings->GetSpeedOptionByName(SpeedComboBox->GetSelectedOption());
        const TOptional<ESizeOption> SizeOption = GameUserSettings->GetSizeOptionByName(SizeComboBox->GetSelectedOption());
        if (SpeedOption.IsSet() && SizeOption.IsSet())
        {
            GameUserSettings->SaveSnakeSettings(*SpeedOption, *SizeOption);
        }
    }
}

void USG_MenuWidget::InitComboBox(TObjectPtr<UComboBoxString> ComboBox, const TArray<FString>& OptionNames, const FString& CurrentOptionName)
{
    check(ComboBox);
    ComboBox->ClearOptions();
    for (const auto& Option : OptionNames)
    {
        ComboBox->AddOption(Option);
    }
    ComboBox->SetSelectedOption(CurrentOptionName);
    ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnComboBoxSelectionChanged);
}
