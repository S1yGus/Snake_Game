// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/Utils/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "World/SG_Grid.h"

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
