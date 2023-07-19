// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/Utils/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "World/SG_Grid.h"
#include "World/SG_WorldTypes.h"
#include "Core/Grid.h"
#include "Core/CoreTypes.h"

using namespace Test;
using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FWorld, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
double GridWorldWidth;
double GridWorldHeight;
ASG_Grid* GridView;
UStaticMeshComponent* GridStaticMeshComp;

END_DEFINE_SPEC(FWorld)

void FWorld::Define()
{
    Describe("World",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("EmptyTestLevel");
                         World = GetTestGameWorld();

                         const uint32 CellSize{10};
                         auto GridModel = MakeShared<Grid>(Dim{40, 40});

                         GridWorldWidth = GridModel->size().width * CellSize;
                         GridWorldHeight = GridModel->size().height * CellSize;

                         const char* GridBPName{"Blueprint '/Game/World/BP_VisualGrid.BP_VisualGrid'"};
                         const auto Origin{FTransform::Identity};
                         GridView = CreateBlueprintDeferred<ASG_Grid>(World, GridBPName, Origin);
                         if (!TestNotNull("Grid view should exist", GridView))
                             return;

                         GridView->SetModel(GridModel, CellSize);
                         GridView->FinishSpawning(Origin);

                         GridStaticMeshComp = GridView->FindComponentByClass<UStaticMeshComponent>();
                     });
                 It("GridTransformShouldBeSetupCorrectly",
                    [this]()
                    {
                        if (!TestNotNull("Grid static mesh should exist", GridStaticMeshComp->GetStaticMesh().Get()))
                            return;

                        const FBox Box = GridStaticMeshComp->GetStaticMesh()->GetBoundingBox();
                        const FVector BoxSize = Box.GetSize();

                        TestTrueExpr(GridStaticMeshComp->GetRelativeLocation().Equals(0.5 * FVector{GridWorldHeight, GridWorldWidth, -BoxSize.Z}));
                        TestTrueExpr(GridStaticMeshComp->GetRelativeScale3D().Equals(FVector{GridWorldHeight / BoxSize.X, GridWorldWidth / BoxSize.Y, 1.0}));
                    });
                 It("ColorsShouldBeSetupCorrectly",
                    [this]()
                    {
                        FSnakeColorsTableRow ColorsSet;
                        ColorsSet.GridBackgroundColor = FLinearColor::Red;
                        ColorsSet.GridWallsColor = FLinearColor::Green;
                        ColorsSet.GridLinesColor = FLinearColor::Yellow;

                        GridView->UpdateColors(ColorsSet);

                        const UMaterialInterface* Material = GridStaticMeshComp->GetMaterial(0);
                        FLinearColor ColorToCheck;

                        Material->GetVectorParameterValue(FName{"BackgroundColor"}, ColorToCheck);
                        TestTrueExpr(ColorToCheck.Equals(ColorsSet.GridBackgroundColor));

                        Material->GetVectorParameterValue(FName{"WallsColor"}, ColorToCheck);
                        TestTrueExpr(ColorToCheck.Equals(ColorsSet.GridWallsColor));

                        Material->GetVectorParameterValue(FName{"LinesColor"}, ColorToCheck);
                        TestTrueExpr(ColorToCheck.Equals(ColorsSet.GridLinesColor));
                    });
             });

    Describe("World",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("GameLevel");
                         World = GetTestGameWorld();
                     });
                 It("OnlyOneGridShouldExist",
                    [this]()
                    {
                        // Arrange
                        TArray<AActor*> Grids;
                        UGameplayStatics::GetAllActorsOfClass(World, ASG_Grid::StaticClass(), Grids);

                        // Assert
                        TestTrueExpr(Grids.Num() == 1 && Grids[0]);
                    });
             });
}

#endif
