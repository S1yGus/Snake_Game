// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/CoreTypes.h"
#include "Core/Grid.h"

using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FCoreGrid, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

Dim GridSize;
TUniquePtr<Grid> CoreGrid;

END_DEFINE_SPEC(FCoreGrid)

void FCoreGrid::Define()
{
    Describe("Core.Grid",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         GridSize = {42, 42};
                         CoreGrid = MakeUnique<Grid>(GridSize);
                     });
                 It("GridSizeMustIncludeWalls",
                    [this]()
                    {
                        TestTrueExpr(CoreGrid->size().width == GridSize.width + 2    //
                                     && CoreGrid->size().height == GridSize.height + 2);
                    });
                 It("HitTest.PositionMightBeUpdatedCorrectly.SnakeNode",
                    [this]()
                    {
                        const Position FirstPoition{1, 1};
                        const Position SecondPoition{2, 1};
                        SnakeList Snake;
                        Snake.AddHead(FirstPoition);
                        Snake.AddTail(SecondPoition);

                        TestTrueExpr(CoreGrid->hitTest(FirstPoition, CellType::Empty));
                        TestTrueExpr(CoreGrid->hitTest(SecondPoition, CellType::Empty));

                        CoreGrid->update(Snake.GetHead(), CellType::Snake);

                        TestTrueExpr(CoreGrid->hitTest(FirstPoition, CellType::Snake));
                        TestTrueExpr(CoreGrid->hitTest(SecondPoition, CellType::Snake));
                    });
             });
}

#endif
