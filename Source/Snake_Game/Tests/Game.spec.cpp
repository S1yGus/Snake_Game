// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/CoreTypes.h"
#include "Core/Game.h"

using namespace Snake;

DEFINE_SPEC(FGameModel, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FGameModel::Define()
{
    Describe("GameModel",
             [this]()
             {
                 It("GridMustExist",
                    [this]()
                    {
                        // Arrange
                        const Settings GameSettings{.gridSize{42, 42}};

                        // Act
                        const Game GameModel{GameSettings};

                        // Assert
                        TestTrueExpr(GameModel.grid().IsValid());
                    });
             });
}

#endif
