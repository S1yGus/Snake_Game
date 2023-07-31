// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/CoreTypes.h"
#include "Core/Game.h"
#include "Core/Grid.h"

using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FIntegration, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

Settings GameSettings;
TUniquePtr<Game> CoreGame;

END_DEFINE_SPEC(FIntegration)

void FIntegration::Define()
{
    Describe("Integration",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         GameSettings = {.gridSize{4, 1}, .gameSpeed{1.0f}, .snake{.defaultSize{3}, .startPosition{3, 1}}};
                         CoreGame = MakeUnique<Game>(GameSettings);
                     });
                 It("FoodShouldn'tBeGeneratedInPlaceOfSnakeHead",
                    [this]()
                    {
                        TestTrueExpr(CoreGame->grid()->hitTest({4, 1}, CellType::Food));

                        CoreGame->update(GameSettings.gameSpeed, Input::defaultInput);

                        TestTrueExpr(CoreGame->grid()->hitTest({1, 1}, CellType::Food));
                    });
             });
}

#endif
