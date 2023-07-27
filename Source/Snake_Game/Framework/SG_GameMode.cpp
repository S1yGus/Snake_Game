// Snake_Game, all rights reserved.

#include "Framework/SG_GameMode.h"
#include "Framework/SG_Pawn.h"
#include "Core/Grid.h"
#include "World/SG_WorldTypes.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "Engine/DataTable.h"
#include "Engine/ExponentialHeightFog.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"

using namespace SnakeGame;

ASG_GameMode::ASG_GameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    if (!GetWorld())
        return;

    // Init game model
    CoreGame = MakeUnique<Game>(MakeSettings());
    check(CoreGame.IsValid());

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

    // Update colors
    check(SnakeColorsTable);
    const auto RowsAmount = SnakeColorsTable->GetRowNames().Num();
    check(RowsAmount != 0);
    ColorsTableIndex = FMath::RandHelper(RowsAmount);
    FindFog();
    UpdateColors();

    // Init pawn
    if (const auto* PC = GetWorld()->GetFirstPlayerController())
    {
        if (auto* Pawn = PC->GetPawn<ASG_Pawn>(); Pawn && CoreGame->grid().IsValid())
        {
            Pawn->UpdateLocation(CoreGame->grid()->size(), CellSize, GridOrigin);
        }
    }

    SetupInput();
}

void ASG_GameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    CoreGame->update(DeltaSeconds, SnakeInput);
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

SnakeGame::Settings ASG_GameMode::MakeSettings() const
{
    checkf(SnakeDefaultSize <= GridSize.X / 2, TEXT("Default snake is too long!"));
    return {.gridSize{GridSize.X, GridSize.Y},    //
            .gameSpeed{GameSpeed},                //
            .snake{.defaultSize{SnakeDefaultSize}, .startPosition{Grid::center({GridSize.X, GridSize.Y})}}};
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
                InputComponen->BindAction(ResetInputAction, ETriggerEvent::Started, this, &ThisClass::OnReset);
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

void ASG_GameMode::OnReset(const FInputActionValue& Value)
{
    CoreGame.Reset(new Game(MakeSettings()));
    check(CoreGame.IsValid());
    GridView->SetModel(CoreGame->grid(), CellSize);
    SnakeView->SetModel(CoreGame->snake(), CellSize, CoreGame->grid()->size());
    SnakeInput = Input::defaultInput;
    NextColor();
}
