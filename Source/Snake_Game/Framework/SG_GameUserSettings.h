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

UENUM()
enum class ECulture : uint8
{
    EN = 0,
    RU
};

USTRUCT()
struct FCultureData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly)
    ECulture Culture;

    UPROPERTY(EditDefaultsOnly)
    FString RepresentationReference;
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
     * @return TArray<FText> Speed option names
     */
    TArray<FText> GetSpeedOptionNames() const;

    /**
     * Returns current game speed option name
     * @return FText Current speed option name
     */
    FText GetCurrentSpeedOptionName() const { return CurrentSpeedOption.Name; }

    /**
     * Returns current game speed option data
     * @return SpeedData Current speed option data
     */
    SnakeGame::SpeedData GetCurrentSpeed() const { return CurrentSpeedOption.Speed; }

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
     * @return TArray<FText> Grid size option names
     */
    TArray<FText> GetSizeOptionNames() const;

    /**
     * Returns current grid size option name
     * @return FText Current grid szie option name
     */
    FText GetCurrentSizeOptionName() const { return CurrentSizeOption.Name; }

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
#pragma region Culture
    /**
     * Returns an array of culture options data
     * @return TArray<FCultureData> Culture options data
     */
    TArray<FCultureData> GetCultureOptions() const { return CultureOptions; }

    /**
     * Saves the given culture
     * @param Culture Culture option
     */
    void SetCurrentCulture(ECulture Culture);
#pragma endregion Culture functions

private:
    struct SpeedData
    {
        FText Name;
        SnakeGame::SpeedData Speed;
    };
    TMap<ESpeedOption, SpeedData> SpeedOptions{{ESpeedOption::Worm, {NSLOCTEXT("Snake_Game_UI", "SpeedWorm_Loc", "Worm"), {.initial = 0.3f, .limit = 0.2f}}},    //
                                               {ESpeedOption::Snake, {NSLOCTEXT("Snake_Game_UI", "SpeedSnake_Loc", "Snake"), {.initial = 0.2f, .limit = 0.1f}}}};
    SpeedData CurrentSpeedOption{SpeedOptions[ESpeedOption::Snake]};

    struct SizeData
    {
        FText Name;
        SnakeGame::Dim Size;
    };
    TMap<ESizeOption, SizeData> SizeOptions{{ESizeOption::Size_30x12, {FText::FromString("30x12"), {30, 12}}},    //
                                            {ESizeOption::Size_40x16, {FText::FromString("40x16"), {40, 16}}},    //
                                            {ESizeOption::Size_50x20, {FText::FromString("50x20"), {50, 20}}}};
    SizeData CurrentSizeOption{SizeOptions[ESizeOption::Size_30x12]};

    TArray<FCultureData> CultureOptions{{ECulture::EN, "Texture2D '/Game/UI/Texture/T_En.T_En'"},    //
                                        {ECulture::RU, "Texture2D '/Game/UI/Texture/T_Ru.T_Ru'"}};

    template <typename T>
    TArray<FText> GetOptionNames(const T& Options) const
    {
        TArray<FText> Names;
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
                                                          return Pair.Value.Name.ToString() == Name;
                                                      }))
        {
            return Found->Key;
        }
        return NullOpt;
    }
};
