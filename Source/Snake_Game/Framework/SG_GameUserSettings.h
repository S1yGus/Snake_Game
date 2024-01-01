// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/CoreTypes.h"
#include "SG_GameUserSettings.generated.h"

UENUM()
enum class ESpeedOption : uint8
{
    Worm = 0,
    Snake
};

UENUM()
enum class ESizeOption : uint8
{
    Size_30x12 = 0,
    Size_40x16,
    Size_50x20,
};

UCLASS()
class SNAKE_GAME_API USG_GameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    USG_GameUserSettings();

    /**
     * Returns pointer to the custom Snake_Game GameUserSettings
     * @return USG_GameUserSettings* Pointer to the custom GameUserSettings
     */
    static USG_GameUserSettings* Get();

    /**
     * Saves the given game speed and grid size
     * @param SpeedOption Game speed option
     * @param SizeOption Grid size option
     */
    void SaveSnakeSettings(ESpeedOption SpeedOption, ESizeOption SizeOption);

#pragma region Game speed
    /**
     * Returns an array of game speed option names
     * @return TArray<FString> Speed option names
     */
    TArray<FString> GetSpeedOptionNames() const;

    /**
     * Returns current game speed option name
     * @return FString Current speed option name
     */
    FString GetCurrentSpeedOptionName() const { return CurrentSpeedOption.Name; }

    /**
     * Returns current game speed option value
     * @return float Current speed option value
     */
    float GetCurrentSpeed() const { return CurrentSpeedOption.Speed; }

    /**
     * Returns the enumeration of game speed option appropriate for the given string option name
     * @param Name Option name
     * @return TOptional<ESpeedOption> Option enumeration
     */
    TOptional<ESpeedOption> GetSpeedOptionByName(const FString& Name) const;
#pragma endregion Game speed functions
#pragma region Grid size
    /**
     * Returns an array of grid size option names
     * @return TArray<FString> Grid size option names
     */
    TArray<FString> GetSizeOptionNames() const;

    /**
     * Returns current grid size option name
     * @return FString Current grid szie option name
     */
    FString GetCurrentSizeOptionName() const { return CurrentSizeOption.Name; }

    /**
     * Returns current grid size option value
     * @return Dim Current grid size option value
     */
    SnakeGame::Dim GetCurrentSize() const { return CurrentSizeOption.Size; }

    /**
     * Returns an enumeration of gird size option appropriate for the given string option name
     * @param Name Option name
     * @return TOptional<ESizeOption> Option enumeration
     */
    TOptional<ESizeOption> GetSizeOptionByName(const FString& Name) const;
#pragma endregion Grid size functions

private:
    struct SpeedData
    {
        FString Name;
        float Speed;
    };
    TMap<ESpeedOption, SpeedData> SpeedOptions{{ESpeedOption::Worm, {"Worm", 0.3f}},    //
                                               {ESpeedOption::Snake, {"Snake", 0.15f}}};
    SpeedData CurrentSpeedOption{SpeedOptions[ESpeedOption::Snake]};

    struct SizeData
    {
        FString Name;
        SnakeGame::Dim Size;
    };
    TMap<ESizeOption, SizeData> SizeOptions{{ESizeOption::Size_30x12, {"30x12", {30, 12}}},    //
                                            {ESizeOption::Size_40x16, {"40x16", {40, 16}}},    //
                                            {ESizeOption::Size_50x20, {"50x20", {50, 20}}}};
    SizeData CurrentSizeOption{SizeOptions[ESizeOption::Size_40x16]};

    template <typename T>
    TArray<FString> GetOptionNames(const T& Options) const
    {
        TArray<FString> Names;
        Algo::Transform(Options, Names,
                        [](const auto& Data)
                        {
                            return Data.Value.Name;
                        });
        return Names;
    }

    template <typename T1, typename T2>
    TOptional<T1> GetOptionByName(const TMap<T1, T2>& Options, const FString& Name) const
    {
        if (const auto* Found = Algo::FindByPredicate(Options,
                                                      [&](const auto& Pair)
                                                      {
                                                          return Pair.Value.Name == Name;
                                                      }))
        {
            return Found->Key;
        }
        return NullOpt;
    }
};
