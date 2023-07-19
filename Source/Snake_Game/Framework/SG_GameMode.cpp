// Snake_Game, all rights reserved.

#include "Framework/SG_GameMode.h"
#include "Framework/SG_Pawn.h"
#include "Core/CoreTypes.h"
#include "Core/Grid.h"
#include "World/SG_Grid.h"
#include "World/SG_WorldTypes.h"
#include "Engine/DataTable.h"
#include "Engine/ExponentialHeightFog.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ExponentialHeightFogComponent.h"

using namespace SnakeGame;

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    if (!GetWorld())
        return;

    // Init game model
    const Settings GameSettings{.gridSize{GridSize.X, GridSize.Y}};
    CoreGame = MakeUnique<Game>(GameSettings);
    check(CoreGame.IsValid());

    // Init grid view
    const auto GridOrigin{FTransform::Identity};
    GridView = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridOrigin);
    check(GridView);
    GridView->InitModel(CoreGame->grid(), CellSize);
    GridView->FinishSpawning(GridOrigin);

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
