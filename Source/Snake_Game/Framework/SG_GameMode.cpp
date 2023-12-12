// Snake_Game, all rights reserved.

#include "Framework/SG_GameMode.h"
#include "Framework/SG_Pawn.h"
#include "Framework/SG_GameUserSettings.h"
#include "Core/Grid.h"
#include "Core/Food.h"
#include "World/SG_WorldTypes.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "World/SG_Food.h"
#include "Engine/DataTable.h"
#include "Engine/ExponentialHeightFog.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "World/SG_WorldUtils.h"
#include "UI/SG_HUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogSnakeGameMode, All, All)

using namespace SnakeGame;

ASG_GameMode::ASG_GameMode()
{
    PrimaryActorTick.bCanEverTick = true;

    HUDClass = ASG_HUD::StaticClass();
    DefaultPawnClass = ASG_Pawn::StaticClass();
}

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    if (!GetWorld())
        return;

    const auto* PC = GetWorld()->GetFirstPlayerController();
    if (!PC)
    {
        UE_LOG(LogSnakeGameMode, Fatal, TEXT("Player Controller is nullptr, game aborted!"));
    }

    auto* Pawn = PC->GetPawn<ASG_Pawn>();
    if (!Pawn)
    {
        UE_LOG(LogSnakeGameMode, Fatal, TEXT("Pawn is nullptr, game aborted!"));
    }

    // Init game model
    CoreGame = MakeShared<Game>(MakeSettings());
    check(CoreGame.IsValid());
    SubscribeOnGameEvent();

    // Init grid view
    const auto GridOrigin{FTransform::Identity};
    GridView = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridOrigin);
    check(GridView);
    GridView->SetModel(CoreGame->grid(), CellSize);
    GridView->FinishSpawning(GridOrigin);

    // Init snake view
    SnakeView = GetWorld()->SpawnActorDeferred<ASG_Snake>(SnakeVisualClass, GridOrigin);
    check(SnakeView);
    SnakeView->SetModel(CoreGame->snake(), CellSize, CoreGame->grid()->size());
    SnakeView->FinishSpawning(GridOrigin);

    // Init food view
    const auto FoodTransform = GridOrigin * FTransform(Utils::PosToVector(CoreGame->food()->position(), CoreGame->grid()->size(), CellSize));
    FoodView = GetWorld()->SpawnActorDeferred<ASG_Food>(FoodVisualClass, FoodTransform);
    check(FoodView);
    FoodView->SetModel(CoreGame->food(), CellSize, CoreGame->grid()->size(), GridOrigin.GetLocation());
    FoodView->UpdateScale(CellSize);
    FoodView->FinishSpawning(FoodTransform);

    // Init pawn
    Pawn->UpdateLocation(CoreGame->grid()->size(), CellSize, GridOrigin);

    // Init HUD
    HUD = PC->GetHUD<ASG_HUD>();
    check(HUD);
    HUD->SetModel(CoreGame);
    HUD->SetKeyNames(Utils::GetActionKeyName(SnakeInputMapping, ResetInputAction), Utils::GetActionKeyName(SnakeInputMapping, BackToMenuInputAction));
    Utils::SetUIInput(GetWorld(), false);

    // Update colors
    check(SnakeColorsTable);
    const int32 RowsAmount = SnakeColorsTable->GetRowNames().Num();
    check(RowsAmount != 0);
    ColorsTableIndex = FMath::RandHelper(RowsAmount);
    FindFog();
    UpdateColors();

    SetupInput();
}

void ASG_GameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    CoreGame->update(DeltaSeconds, SnakeInput);
}

void ASG_GameMode::Reset()
{
    CoreGame = MakeShared<Game>(MakeSettings());
    check(CoreGame.IsValid());
    SubscribeOnGameEvent();
    GridView->SetModel(CoreGame->grid(), CellSize);
    SnakeView->SetModel(CoreGame->snake(), CellSize, CoreGame->grid()->size());
    FoodView->SetModel(CoreGame->food(), CellSize, CoreGame->grid()->size(), GridView->GetActorLocation());
    FoodView->RestartScaling();
    HUD->SetModel(CoreGame);
    SnakeInput = Input::defaultInput;
    NextColor();
    Utils::SetUIInput(GetWorld(), false);
}

void ASG_GameMode::BackToMenu()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(this, MenuLevel);
}

void ASG_GameMode::NextColor()
{
    ColorsTableIndex = (ColorsTableIndex + 1) % SnakeColorsTable->GetRowNames().Num();
    UpdateColors();
}

void ASG_GameMode::UpdateColors()
{
    TArray<FSnakeColorsTableRow*> SnakeColorsTableRows;
    SnakeColorsTable->GetAllRows<FSnakeColorsTableRow>({}, SnakeColorsTableRows);
    const auto* ColorsSet = SnakeColorsTableRows[ColorsTableIndex];
    GridView->UpdateColors(*ColorsSet);
    SnakeView->UpdateColors(*ColorsSet);
    FoodView->UpdateColor(ColorsSet->FoodColor);

    if (Fog && Fog->GetComponent())
    {
        Fog->GetComponent()->SkyAtmosphereAmbientContributionColorScale = ColorsSet->FogColor;
        Fog->MarkComponentsRenderStateDirty();
    }
}

void ASG_GameMode::FindFog()
{
    TArray<AActor*> Fogs;
    UGameplayStatics::GetAllActorsOfClass(this, AExponentialHeightFog::StaticClass(), Fogs);
    check(Fogs.Num() == 1);
    Fog = Cast<AExponentialHeightFog>(Fogs[0]);
}

Settings ASG_GameMode::MakeSettings() const
{
    checkf(SnakeDefaultSize <= GridSize.X / 2, TEXT("Default snake is too long!"));

    const auto* GameUserSettings = USG_GameUserSettings::Get();
    if (!GameUserSettings)
    {
        UE_LOG(LogSnakeGameMode, Fatal, TEXT("Snake GameUserSettings not found, game aborted!"));
    }

    Settings GameSettings;
#if WITH_EDITOR
    if (bOverrideUserSettings)
    {
        GameSettings.gameSpeed = GameSpeed;
        GameSettings.gridSize = Dim{GridSize.X, GridSize.Y};
    }
    else
#endif
    {
        GameSettings.gameSpeed = GameUserSettings->GetCurrentSpeed();
        GameSettings.gridSize = GameUserSettings->GetCurrentSize();
    }
    GameSettings.snake = {.defaultSize{SnakeDefaultSize},    //
                          .startPosition{Grid::center({GameSettings.gridSize.width, GameSettings.gridSize.height})}};
    return GameSettings;
}

void ASG_GameMode::SetupInput()
{
    if (GetWorld())
    {
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
            {
                if (auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
                {
                    InputSystem->AddMappingContext(SnakeInputMapping, 0);
                }
            }

            if (auto* InputComponen = Cast<UEnhancedInputComponent>(PC->InputComponent))
            {
                InputComponen->BindAction(MoveForwardInputAction, ETriggerEvent::Started, this, &ThisClass::OnMoveForward);
                InputComponen->BindAction(MoveRightInputAction, ETriggerEvent::Started, this, &ThisClass::OnMoveRight);
                InputComponen->BindAction(ResetInputAction, ETriggerEvent::Started, this, &ThisClass::Reset);
                InputComponen->BindAction(BackToMenuInputAction, ETriggerEvent::Started, this, &ThisClass::BackToMenu);
            }
        }
    }
}

void ASG_GameMode::OnMoveForward(const FInputActionValue& Value)
{
    if (const float FloatValue = Value.Get<float>())
    {
        SnakeInput = Input{0, static_cast<int8>(FloatValue)};
    }
}

void ASG_GameMode::OnMoveRight(const FInputActionValue& Value)
{
    if (const float FloatValue = Value.Get<float>())
    {
        SnakeInput = Input{static_cast<int8>(FloatValue), 0};
    }
}

void ASG_GameMode::SubscribeOnGameEvent()
{
    CoreGame->subscribeOnGameEvent(
        [&](GameEvent Event)
        {
            switch (Event)
            {
                case GameEvent::GameOver:
                    SnakeView->Teardown();
                    FoodView->Teardown();
                    FoodView->SetActorHiddenInGame(true);
                    Utils::SetUIInput(GetWorld(), true);
#if !UE_BUILD_SHIPPING
                    UE_LOG(LogSnakeGameMode, Display, TEXT("Game Over!"));
                    UE_LOG(LogSnakeGameMode, Display, TEXT("Score: %d"), CoreGame->score());
#endif
                    break;
                case GameEvent::GameCompleted:
#if !UE_BUILD_SHIPPING
                    UE_LOG(LogSnakeGameMode, Display, TEXT("Game Completed!"));
                    UE_LOG(LogSnakeGameMode, Display, TEXT("Score: %d"), CoreGame->score());
#endif
                    break;
                case GameEvent::FoodTaken:
                    FoodView->Teardown();
                    FoodView->RestartScaling();
                    break;
                default:
                    break;
            }
        });
}
