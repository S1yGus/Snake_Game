// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/CoreTypes.h"
#include "Core/Game.h"
#include "Core/Grid.h"

using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FCoreGame, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

Position SnakeStartPosition;
Settings GameSettings;
TUniquePtr<Game> CoreGame;

END_DEFINE_SPEC(FCoreGame)

void FCoreGame::Define()
{
    Describe("Core.Game",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         SnakeStartPosition = {2, 2};
                         GameSettings = {.gridSize{42, 42}, .speed{.initial = 1.0f, .limit = 1.0f}, .snake{2, SnakeStartPosition}};
                         CoreGame = MakeUnique<Game>(GameSettings);
                     });
                 It("GridMustExist",
                    [this]()
                    {
                        TestTrueExpr(CoreGame->grid().IsValid());
                    });
                 It("SnakeMustExist",
                    [this]()
                    {
                        TestTrueExpr(CoreGame->snake().IsValid());
                    });
                 It("FoodMustExist",
                    [this]()
                    {
                        TestTrueExpr(CoreGame->food().IsValid());
                    });
                 It("Update.SnakeShouldn'tMoveWhenDeltaLessThanGameSpeed",
                    [this]()
                    {
                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition, CellType::Snake));
                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition - Position{1, 0}, CellType::Snake));

                        CoreGame->update(GameSettings.speed.initial / 2, Input::defaultInput);

                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition, CellType::Snake));
                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition - Position{1, 0}, CellType::Snake));
                    });
                 It("Update.SnakeShouldMoveWhenDeltaEqualGameSpeed",
                    [this]()
                    {
                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition, CellType::Snake));
                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition - Position{1, 0}, CellType::Snake));

                        CoreGame->update(GameSettings.speed.initial, Input::defaultInput);

                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition + Input::defaultInput, CellType::Empty));
                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition - Position{1, 0} + Input::defaultInput, CellType::Snake));
                    });
                 It("Update.SnakeShouldMoveWhenDeltaAccumulates",
                    [this]()
                    {
                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition, CellType::Snake));
                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition - Position{1, 0}, CellType::Snake));

                        CoreGame->update(GameSettings.speed.initial / 2, Input::defaultInput);

                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition, CellType::Snake));
                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition - Position{1, 0}, CellType::Snake));

                        CoreGame->update(GameSettings.speed.initial / 2, Input::defaultInput);

                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition + Input::defaultInput, CellType::Empty));
                        TestTrueExpr(CoreGame->grid()->hitTest(SnakeStartPosition - Position{1, 0} + Input::defaultInput, CellType::Snake));
                    });
             });
}

#endif
