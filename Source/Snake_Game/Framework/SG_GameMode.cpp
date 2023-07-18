// Snake_Game, all rights reserved.

#include "Framework/SG_GameMode.h"
#include "Framework/SG_Pawn.h"
#include "Core/CoreTypes.h"
#include "Core/Grid.h"
#include "World/SG_Grid.h"

using namespace Snake;

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

    // Init pawn
    if (const auto* PC = GetWorld()->GetFirstPlayerController())
    {
        if (auto* Pawn = PC->GetPawn<ASG_Pawn>(); Pawn && CoreGame->grid().IsValid())
        {
            Pawn->UpdateLocation(CoreGame->grid()->size(), CellSize, GridOrigin);
        }
    }
}
