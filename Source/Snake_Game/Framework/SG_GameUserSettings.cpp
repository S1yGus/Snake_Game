// Snake_Game, all rights reserved.

#include "Framework/SG_GameUserSettings.h"
#include "Kismet/KismetInternationalizationLibrary.h"

static FString CultureToString(ECulture Culture)
{
    switch (Culture)
    {
        case ECulture::RU:
            return "ru-RU";
        default:
            return "en";
    }
}

USG_GameUserSettings::USG_GameUserSettings()
{
    check(SpeedOptions.Num() == StaticEnum<ESpeedOption>()->NumEnums() - 1);
    check(SizeOptions.Num() == StaticEnum<ESizeOption>()->NumEnums() - 1);
}

USG_GameUserSettings* USG_GameUserSettings::Get()
{
    return GEngine ? Cast<USG_GameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

void USG_GameUserSettings::SaveSnakeSettings(ESpeedOption SpeedOption, ESizeOption SizeOption)
{
    CurrentSpeedOption = SpeedOptions[SpeedOption];
    CurrentSizeOption = SizeOptions[SizeOption];
}

TArray<FText> USG_GameUserSettings::GetSpeedOptionNames() const
{
    return GetOptionNames(SpeedOptions);
}

TOptional<ESpeedOption> USG_GameUserSettings::GetSpeedOptionByName(const FString& Name) const
{
    return GetOptionByName(SpeedOptions, Name);
}

TArray<FText> USG_GameUserSettings::GetSizeOptionNames() const
{
    return GetOptionNames(SizeOptions);
}

TOptional<ESizeOption> USG_GameUserSettings::GetSizeOptionByName(const FString& Name) const
{
    return GetOptionByName(SizeOptions, Name);
}

void USG_GameUserSettings::SetCurrentCulture(ECulture Culture)
{
    UKismetInternationalizationLibrary::SetCurrentCulture(CultureToString(Culture), true);
}
