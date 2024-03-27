// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/CoreTypes.h"
#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"

using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FIntegration, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

Settings GameSettings;
TUniquePtr<Game> CoreGame;

END_DEFINE_SPEC(FIntegration)

class StubPositionRandomizer : public IPositionRandomizer
{
public:
    StubPositionRandomizer(const TArray<Position>& InPositions) : Positions{InPositions} {}

    virtual TOptional<Position> randomEmptyPosition(const TArray<CellType>& cells, const Dim& size) const override
    {
        if (Index < Positions.Num())
        {
            return Positions[Index++];
        }

        return NullOpt;
    }

private:
    TArray<Position> Positions;
    mutable int32 Index{0};
};

void FIntegration::Define()
{
    Describe("Integration",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         GameSettings = {.gridSize{4, 1}, .speed{.initial = 1.0f, .limit = 1.0f}, .snake{.defaultSize{3}, .startPosition{3, 1}}};
                         CoreGame = MakeUnique<Game>(GameSettings);
                     });
                 It("FoodShouldn'tBeGeneratedInPlaceOfSnakeHead",
                    [this]()
                    {
                        TestTrueExpr(CoreGame->grid()->hitTest({4, 1}, CellType::Food));

                        CoreGame->update(GameSettings.speed.initial, {1, 0});    // Move forward and take food

                        TestTrueExpr(CoreGame->grid()->hitTest({1, 1}, CellType::Food));
                    });
             });

    Describe("Integration",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         const TArray<Position> Positions = {{3, 1}};    // The first position is valid, the second position is not set
                         GameSettings = {.gridSize{3, 1},
                                         .speed{.initial = 1.0f, .limit = 1.0f},
                                         .snake{.defaultSize{2}, .startPosition{2, 1}},
                                         .positionRandomizer = MakeShared<StubPositionRandomizer>(Positions)};
                         CoreGame = MakeUnique<Game>(GameSettings);
                     });
                 It("GameCanBeCompleted",
                    [this]()
                    {
                        bool bGameCompleted{false};
                        CoreGame->subscribeOnGameEvent(
                            [&](GameEvent Event)
                            {
                                if (Event == GameEvent::GameCompleted)
                                {
                                    bGameCompleted = true;
                                }
                            });

                        CoreGame->update(GameSettings.speed.initial, {1, 0});    // Move forward

                        TestTrueExpr(bGameCompleted);
                    });
             });

    Describe("Integration",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         TArray<Position> Positions;
                         Positions.Init(Position::zero, 6);
                         GameSettings = {.gridSize{10, 10},
                                         .speed{.initial = 1.0f, .limit = 1.0f},
                                         .snake{.defaultSize{4}, .startPosition{Grid::center({10, 10})}},
                                         .positionRandomizer = MakeShared<StubPositionRandomizer>(Positions)};
                         CoreGame = MakeUnique<Game>(GameSettings);
                     });
                 It("SnakeShouldMoveCorrectlyNextToIt'sTail",
                    [this]()
                    {
                        bool bGameOver{false};
                        CoreGame->subscribeOnGameEvent(
                            [&](GameEvent Event)
                            {
                                if (Event == GameEvent::GameOver)
                                {
                                    bGameOver = true;
                                }
                            });

                        CoreGame->update(GameSettings.speed.initial, {0, 1});     // Move down
                        CoreGame->update(GameSettings.speed.initial, {-1, 0});    // Move left
                        CoreGame->update(GameSettings.speed.initial, {0, -1});    // Move up

                        TestTrueExpr(!bGameOver);    // The snake shouldn't bite its tail
                    });
                 It("GameCanBeOver",
                    [this]()
                    {
                        bool bGameOver{false};
                        CoreGame->subscribeOnGameEvent(
                            [&](GameEvent Event)
                            {
                                if (Event == GameEvent::GameOver)
                                {
                                    bGameOver = true;
                                }
                            });

                        for (uint32 i = 0; i != GameSettings.gridSize.width / 2; ++i)
                        {
                            CoreGame->update(GameSettings.speed.initial, {1, 0});    // Move forward
                        }

                        TestTrueExpr(bGameOver);
                    });
             });

    Describe("Integration",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         const TArray<Position> Positions = {{5, 3}, {5, 5}, Position::zero};
                         GameSettings = {.gridSize{5, 5},
                                         .speed{.initial = 1.0f, .limit = 1.0f},
                                         .snake{.defaultSize{4}, .startPosition{4, 3}},
                                         .positionRandomizer = MakeShared<StubPositionRandomizer>(Positions)};
                         CoreGame = MakeUnique<Game>(GameSettings);
                     });
                 It("FoodCanBeTaken",
                    [this]()
                    {
                        uint32 Score{0};
                        CoreGame->subscribeOnGameEvent(
                            [&](GameEvent Event)
                            {
                                if (Event == GameEvent::FoodTaken)
                                {
                                    ++Score;
                                }
                            });

                        TestTrueExpr(CoreGame->score() == 0);
                        TestTrueExpr(Score == 0);

                        CoreGame->update(GameSettings.speed.initial, {1, 0});    // Move forward

                        TestTrueExpr(CoreGame->score() == 1);
                        TestTrueExpr(Score == 1);

                        CoreGame->update(GameSettings.speed.initial, {0, 1});    // Move down

                        TestTrueExpr(CoreGame->score() == 1);
                        TestTrueExpr(Score == 1);

                        CoreGame->update(GameSettings.speed.initial, {0, 1});    // Move down

                        TestTrueExpr(CoreGame->score() == 2);
                        TestTrueExpr(Score == 2);
                    });
             });

    Describe("Integration",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         const TArray<Position> Positions = {{5, 1}, {6, 1}, Position::zero};
                         GameSettings = {.gridSize{7, 1},
                                         .speed{.initial = 3.0f, .limit = 2.0f, .boost = 1.0f},
                                         .snake{.defaultSize{4}, .startPosition{4, 1}},
                                         .positionRandomizer = MakeShared<StubPositionRandomizer>(Positions)};
                         CoreGame = MakeUnique<Game>(GameSettings);
                     });
                 It("GameSpeedCanBeBoosted",
                    [this]()
                    {
                        const auto GameSpeedAfterBoost = GameSettings.speed.initial - GameSettings.speed.boost;
                        TestTrueExpr(CoreGame->snake()->head() == Position(4, 1));
                        CoreGame->update(GameSpeedAfterBoost, {1, 0});    // Cannot move forward until boost
                        TestTrueExpr(CoreGame->snake()->head() == Position(4, 1));
                        CoreGame->update(GameSettings.speed.initial, {1, 0});    // Move forward and get a boost
                        TestTrueExpr(CoreGame->snake()->head() == Position(5, 1));
                        CoreGame->update(GameSpeedAfterBoost, {1, 0});    // Move forward
                        TestTrueExpr(CoreGame->snake()->head() == Position(6, 1));
                    });
                 It("GameSpeedCannotBeLowerThanLimit",
                    [this]()
                    {
                        TestTrueExpr(CoreGame->snake()->head() == Position(4, 1));
                        CoreGame->update(GameSettings.speed.initial, {1, 0});    // Move forward and get a boost
                        TestTrueExpr(CoreGame->snake()->head() == Position(5, 1));
                        CoreGame->update(GameSettings.speed.initial - GameSettings.speed.boost, {1, 0});    // Move forward and get a boost
                        TestTrueExpr(CoreGame->snake()->head() == Position(6, 1));
                        CoreGame->update(GameSettings.speed.initial - GameSettings.speed.boost * 2, {1, 0});    // Cannot move forward, speed is below the limit
                        TestTrueExpr(CoreGame->snake()->head() == Position(6, 1));
                        CoreGame->update(GameSettings.speed.limit, {1, 0});    // Move forward
                        TestTrueExpr(CoreGame->snake()->head() == Position(7, 1));
                    });
             });
}

#endif
