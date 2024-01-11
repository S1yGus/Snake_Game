// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/Utils/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "World/SG_Food.h"

using namespace Test;

BEGIN_DEFINE_SPEC(FWorld, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;

END_DEFINE_SPEC(FWorld)

void FWorld::Define()
{
    Describe("World",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("/Game/Levels/GameLevel");
                         World = GetTestGameWorld();
                     });
                 It("OnlyOneGridShouldExist",
                    [this]()
                    {
                        TArray<AActor*> Grids;
                        UGameplayStatics::GetAllActorsOfClass(World, ASG_Grid::StaticClass(), Grids);

                        TestTrueExpr(Grids.Num() == 1 && Grids[0]);

                        SpecCloseLevel(World);
                    });
                 It("OnlyOneSnakeShouldExist",
                    [this]()
                    {
                        TArray<AActor*> Snakes;
                        UGameplayStatics::GetAllActorsOfClass(World, ASG_Snake::StaticClass(), Snakes);

                        TestTrueExpr(Snakes.Num() == 1 && Snakes[0]);

                        SpecCloseLevel(World);
                    });
                 It("OnlyOneFoodShouldExist",
                    [this]()
                    {
                        TArray<AActor*> Foods;
                        UGameplayStatics::GetAllActorsOfClass(World, ASG_Food::StaticClass(), Foods);

                        TestTrueExpr(Foods.Num() == 1 && Foods[0]);

                        SpecCloseLevel(World);
                    });
             });
}

#endif
