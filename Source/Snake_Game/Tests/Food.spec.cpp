// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/CoreTypes.h"
#include "Core/Food.h"

using namespace SnakeGame;

DEFINE_SPEC(FCoreFood, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FCoreFood::Define()
{
    Describe("Core.Food",
             [this]()
             {
                 It("FoodPositionCanBeSet",
                    [this]()
                    {
                        Food Food;
                        Position Position{1, 1};
                        Food.setPosition(Position);

                        TestTrueExpr(Food.position() == Position);
                    });
             });
}

#endif
