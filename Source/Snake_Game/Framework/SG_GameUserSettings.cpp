// Snake_Game, all rights reserved.

#include "Framework/SG_GameUserSettings.h"

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

TArray<FString> USG_GameUserSettings::GetSpeedOptionNames() const
{
    return GetOptionNames(SpeedOptions);
}

TOptional<ESpeedOption> USG_GameUserSettings::GetSpeedOptionByName(const FString& Name) const
{
    return GetOptionByName(SpeedOptions, Name);
}

TArray<FString> USG_GameUserSettings::GetSizeOptionNames() const
{
    return GetOptionNames(SizeOptions);
}

TOptional<ESizeOption> USG_GameUserSettings::GetSizeOptionByName(const FString& Name) const
{
    return GetOptionByName(SizeOptions, Name);
}
