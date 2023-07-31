// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/CoreTypes.h"
#include "Core/Snake.h"

using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FCoreSnake, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

Position StartPosition;
Settings::Snake Settings;
TUniquePtr<Snake> CoreSnake;

END_DEFINE_SPEC(FCoreSnake)

void FCoreSnake::Define()
{
    Describe("Core.Snake",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         StartPosition = {42, 42};
                         Settings = {.defaultSize{10}, .startPosition{StartPosition}};
                         CoreSnake = MakeUnique<Snake>(Settings);
                     });
                 It("MightBeInitCorrectly",
                    [this]()
                    {
                        TestTrueExpr(CoreSnake->links().Num() == Settings.defaultSize);
                        TestTrueExpr(CoreSnake->head() == StartPosition);

                        auto* Link = CoreSnake->links().GetHead();
                        Position NextPosition = StartPosition;
                        while (Link)
                        {
                            TestTrueExpr(Link->GetValue() == NextPosition);

                            Link = Link->GetNextNode();
                            NextPosition = {NextPosition.x - 1, NextPosition.y};
                        }
                    });
                 It("Movement.CanBeMovedRight",
                    [this]()
                    {
                        Input SnakeInput{1, 0};
                        CoreSnake->move(SnakeInput);

                        TestTrueExpr(CoreSnake->head() == (StartPosition + SnakeInput));

                        auto* Link = CoreSnake->body();
                        Position NextPosition = StartPosition;
                        while (Link)
                        {
                            TestTrueExpr(Link->GetValue() == NextPosition);

                            Link = Link->GetNextNode();
                            NextPosition = {NextPosition.x - 1, NextPosition.y};
                        }
                    });
                 It("Movement.CannotBeMovedLeftByDefault",
                    [this]()
                    {
                        Input SnakeInput{-1, 0};
                        CoreSnake->move(SnakeInput);

                        auto* Link = CoreSnake->links().GetHead();
                        Position NextPosition = StartPosition + Input{1, 0};
                        while (Link)
                        {
                            TestTrueExpr(Link->GetValue() == NextPosition);

                            Link = Link->GetNextNode();
                            NextPosition = {NextPosition.x - 1, NextPosition.y};
                        }
                    });
                 It("Movement.CanBeMovedUp",
                    [this]()
                    {
                        Input SnakeInput{0, -1};
                        CoreSnake->move(SnakeInput);

                        TestTrueExpr(CoreSnake->head() == (StartPosition + SnakeInput));

                        auto* Link = CoreSnake->body();
                        Position NextPosition = StartPosition;
                        while (Link)
                        {
                            TestTrueExpr(Link->GetValue() == NextPosition);

                            Link = Link->GetNextNode();
                            NextPosition = {NextPosition.x - 1, NextPosition.y};
                        }
                    });
                 It("Movement.CanBeMovedDown",
                    [this]()
                    {
                        Input SnakeInput{0, 1};
                        CoreSnake->move(SnakeInput);

                        TestTrueExpr(CoreSnake->head() == (StartPosition + SnakeInput));

                        auto* Link = CoreSnake->body();
                        Position NextPosition = StartPosition;
                        while (Link)
                        {
                            TestTrueExpr(Link->GetValue() == NextPosition);

                            Link = Link->GetNextNode();
                            NextPosition = {NextPosition.x - 1, NextPosition.y};
                        }
                    });
                 It("SnakeCanIncreaseOnMove",
                    [this]()
                    {
                        for (uint32 i = 1; i != 10; ++i)
                        {
                            CoreSnake->increase();
                            CoreSnake->move(Input::defaultInput);
                            TestTrueExpr(CoreSnake->links().Num() == Settings.defaultSize + i);
                        }
                    });
                 It("SnakeCan'tIncreaseWithoutMove",
                    [this]()
                    {
                        for (uint32 i = 1; i != 10; ++i)
                        {
                            CoreSnake->increase();
                            TestTrueExpr(CoreSnake->links().Num() == Settings.defaultSize);
                        }
                    });
             });
}

#endif
