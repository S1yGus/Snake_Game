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
                        Snake.AddTail(FirstPoition);
                        Snake.AddTail(SecondPoition);

                        TestTrueExpr(CoreGrid->hitTest(FirstPoition, CellType::Empty));
                        TestTrueExpr(CoreGrid->hitTest(SecondPoition, CellType::Empty));

                        CoreGrid->update(Snake.GetHead(), CellType::Snake);

                        TestTrueExpr(CoreGrid->hitTest(FirstPoition, CellType::Snake));
                        TestTrueExpr(CoreGrid->hitTest(SecondPoition, CellType::Snake));
                    });
                 It("HitTest.PositionMightBeUpdatedCorrectly.Position",
                    [this]()
                    {
                        const Position Poition{1, 1};

                        TestTrueExpr(CoreGrid->hitTest(Poition, CellType::Empty));

                        CoreGrid->update(Poition, CellType::Food);

                        TestTrueExpr(CoreGrid->hitTest(Poition, CellType::Food));
                    });
                 It("CellsMightBeClearCorrectly",
                    [this]()
                    {
                        const Position FirstPoition{1, 1};
                        const Position SecondPoition{2, 1};

                        TestTrueExpr(CoreGrid->hitTest(FirstPoition, CellType::Empty));
                        TestTrueExpr(CoreGrid->hitTest(SecondPoition, CellType::Empty));

                        CoreGrid->update(FirstPoition, CellType::Food);

                        TestTrueExpr(CoreGrid->hitTest(FirstPoition, CellType::Food));
                        TestTrueExpr(CoreGrid->hitTest(SecondPoition, CellType::Empty));

                        CoreGrid->update(SecondPoition, CellType::Food);

                        TestTrueExpr(CoreGrid->hitTest(FirstPoition, CellType::Empty));
                        TestTrueExpr(CoreGrid->hitTest(SecondPoition, CellType::Food));
                    });
                 It("GridCenterShouldBeCalculateCorrectly",
                    [this]()
                    {
                        TestTrueExpr(Grid::center({13, 7}) == Position(7, 4));
                        TestTrueExpr(Grid::center({12, 6}) == Position(7, 4));
                    });
             });

    Describe("Core.Grid",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         GridSize = {2, 2};
                         CoreGrid = MakeUnique<Grid>(GridSize);
                     });
                 It("EmptyRandomPositionCanBeFound",
                    [this]()
                    {
                        SnakeList Snake;
                        Snake.AddTail({1, 2});
                        Snake.AddTail({2, 2});
                        Snake.AddTail({2, 1});
                        CoreGrid->update(Snake.GetHead(), CellType::Snake);

                        TOptional<Position> FoundPosition = CoreGrid->randomEmptyPosition();

                        TestTrueExpr(FoundPosition.IsSet());
                        TestTrueExpr(*FoundPosition == Position(1, 1));
                    });
                 It("EmptyRandomPositionCan'tBeFound",
                    [this]()
                    {
                        SnakeList Snake;
                        Snake.AddTail({1, 1});
                        Snake.AddTail({1, 2});
                        Snake.AddTail({2, 2});
                        Snake.AddTail({2, 1});
                        CoreGrid->update(Snake.GetHead(), CellType::Snake);

                        TOptional<Position> FoundPosition = CoreGrid->randomEmptyPosition();

                        TestTrueExpr(!FoundPosition.IsSet());
                    });
             });
}

#endif
