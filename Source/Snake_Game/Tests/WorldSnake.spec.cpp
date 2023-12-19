// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/Utils/TestUtils.h"
#include "Core/Snake.h"
#include "World/SG_Snake.h"
#include "World/SG_SnakeLink.h"
#include "Kismet/GameplayStatics.h"

using namespace Test;
using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FWorldSnake, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
Settings::Snake Settings;
TSharedPtr<Snake> SnakeModel;
ASG_Snake* SnakeView;

END_DEFINE_SPEC(FWorldSnake)

void FWorldSnake::Define()
{
    Describe("World.Snake",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("/Game/Tests/EmptyTestLevel");
                         World = GetTestGameWorld();

                         Settings = {.defaultSize = 3, .startPosition = {1, 1}};
                         SnakeModel = MakeShared<Snake>(Settings);

                         const char* SnakeBPName{"Blueprint '/Game/World/BP_Snake.BP_Snake'"};
                         const auto Origin{FTransform::Identity};
                         SnakeView = CreateBlueprintDeferred<ASG_Snake>(World, SnakeBPName, Origin);
                         if (!TestNotNull("Snake view should exist", SnakeView))
                             return;

                         const uint32 CellSize{10};
                         const Dim GridSize{40, 40};
                         SnakeView->SetModel(SnakeModel, CellSize, GridSize);
                         SnakeView->FinishSpawning(Origin);
                     });
                 xIt("NumberOfWorldLinksMustMatchDefaultSnakeSize",    // Deprecated after introduce the object pool
                     [this]()
                     {
                         TArray<AActor*> Links;
                         UGameplayStatics::GetAllActorsOfClass(World, ASG_SnakeLink::StaticClass(), Links);

                         TestTrueExpr(Links.Num() == Settings.defaultSize);

                         SpecCloseLevel(World);
                     });
                 xIt("NumberOfWorldLinksMustMatchSnakeSizeAfterIncrease",    // Deprecated after introduce the object pool
                     [this]()
                     {
                         SnakeModel->increase();
                         SnakeModel->move(Input::defaultInput);

                         ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
                             [this]()
                             {
                                 TArray<AActor*> Links;
                                 UGameplayStatics::GetAllActorsOfClass(World, ASG_SnakeLink::StaticClass(), Links);

                                 TestTrueExpr(Links.Num() == Settings.defaultSize + 1);

                                 SpecCloseLevel(World);
                             }));
                     });
             });
}

#endif
