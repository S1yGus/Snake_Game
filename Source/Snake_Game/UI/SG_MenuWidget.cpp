// Snake_Game, all rights reserved.

#include "UI/SG_MenuWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Framework/SG_GameUserSettings.h"
#include "UI/SG_CultureButton.h"

void USG_MenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(StartGameButton);
    StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGame);
    check(QuitGameButton);
    QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitGame);
    check(ToggleScreenModeButton);
    ToggleScreenModeButton->OnClicked.AddDynamic(this, &ThisClass::OnToggleScreenMode);

    if (auto* GameUserSettings = USG_GameUserSettings::Get())
    {
        InitComboBox(SpeedComboBox, GameUserSettings->GetSpeedOptionNames(), GameUserSettings->GetCurrentSpeedOptionName());
        InitComboBox(SizeComboBox, GameUserSettings->GetSizeOptionNames(), GameUserSettings->GetCurrentSizeOptionName());
        InitCultureButtonsBox(GameUserSettings);
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

void USG_MenuWidget::OnToggleScreenMode()
{
    if (auto* GameUserSettings = USG_GameUserSettings::Get())
    {
        const EWindowMode::Type CurrentMode = GameUserSettings->GetFullscreenMode();
        if (TArray<FIntPoint> Resolutions; CurrentMode != EWindowMode::Windowed)
        {
            UKismetSystemLibrary::GetConvenientWindowedResolutions(Resolutions);
            if (!Resolutions.IsEmpty())
            {
                GameUserSettings->SetScreenResolution(Resolutions[0]);
            }
        }
        GameUserSettings->SetFullscreenMode(CurrentMode == EWindowMode::Type::Windowed ? EWindowMode::Type::WindowedFullscreen : EWindowMode::Type::Windowed);
        GameUserSettings->ApplyResolutionSettings(false);
    }
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

void USG_MenuWidget::InitComboBox(TObjectPtr<UComboBoxString> ComboBox, const TArray<FText>& OptionNames, const FText& CurrentOptionName)
{
    check(ComboBox);
    ComboBox->ClearOptions();
    for (const auto& Option : OptionNames)
    {
        ComboBox->AddOption(Option.ToString());
    }
    ComboBox->SetSelectedOption(CurrentOptionName.ToString());
    ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnComboBoxSelectionChanged);
}

void USG_MenuWidget::InitCultureButtonsBox(TObjectPtr<USG_GameUserSettings> GameUserSettings)
{
    check(CultureButtonsBox);
    CultureButtonsBox->ClearChildren();
    for (const auto& CultureOption : GameUserSettings->GetCultureOptions())
    {
        auto* CultureButton = CreateWidget<USG_CultureButton>(this, CultureButtonClass);
        check(CultureButton);
        CultureButton->Init(CultureOption);
        CultureButton->OnClickedCultureButton.BindLambda(
            [this, GameUserSettings](ECulture Culture)
            {
                GameUserSettings->SetCurrentCulture(Culture);
                UGameplayStatics::OpenLevel(this, FName{*GetWorld()->GetName()}, false);
            });
        CultureButtonsBox->AddChild(CultureButton);
    }
}
