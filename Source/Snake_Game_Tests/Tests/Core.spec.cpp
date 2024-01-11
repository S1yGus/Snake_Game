// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/CoreTypes.h"

using namespace SnakeGame;

DEFINE_SPEC(FCore, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority)

void FCore::Define()
{
    Describe("Core.PositionRandomizer",
             [this]()
             {
                 It("EmptyCellMustBeFound",
                    [this]()
                    {
                        const Dim Size{.width = 42,    //
                                       .height = 10};
                        TArray<CellType> Cells;
                        Cells.Init(CellType::Empty, Size.width * Size.height);

                        // Init walls
                        for (uint32 y = 0; y != Size.height; ++y)
                        {
                            for (uint32 x = 0; x != Size.width; ++x)
                            {
                                if (y == 0 || y == Size.height - 1 || x == 0 || x == Size.width - 1)
                                {
                                    Cells[posToIndex(x, y, Size.width)] = CellType::Wall;
                                }
                            }
                        }

                        PositionRandomizer Randomizer;
                        for (uint32 i = 0; i < 10000; ++i)
                        {
                            const auto RandomPosition = Randomizer.randomEmptyPosition(Cells, Size);
                            TestTrueExpr(RandomPosition.IsSet());
                            TestTrueExpr(RandomPosition->x > 0 && RandomPosition->x < Size.width - 1);
                            TestTrueExpr(RandomPosition->y > 0 && RandomPosition->y < Size.height - 1);
                        }
                    });
             });
}

#endif
