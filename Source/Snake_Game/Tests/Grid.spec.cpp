// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/CoreTypes.h"
#include "Core/Grid.h"

using namespace Snake;

DEFINE_SPEC(FGridModel, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FGridModel::Define()
{
    Describe("GridModel",
             [this]()
             {
                 It("GridSizeMustIncludeWalls",
                    [this]()
                    {
                        // Arrange
                        const Dim InitGridSize{42, 42};

                        // Act
                        const Grid GridModel{InitGridSize};

                        // Assert
                        TestTrueExpr(GridModel.size().width == InitGridSize.width + 2    //
                                     && GridModel.size().height == InitGridSize.height + 2);
                    });
             });
}

#endif
