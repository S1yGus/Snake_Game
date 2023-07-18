// Snake_Game, all rights reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/Utils/TestUtils.h"
#include "Framework/SG_GameMode.h"
#include "Framework/SG_Pawn.h"

using namespace Test;

BEGIN_DEFINE_SPEC(FFramework, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;

END_DEFINE_SPEC(FFramework)

void FFramework::Define()
{
    Describe("Framework",
             [this]()
             {
                 BeforeEach(
                     [this]()
                     {
                         AutomationOpenMap("GameLevel");
                         World = GetTestGameWorld();
                     });

                 It("ClassesMustBeSetupCorrectly",
                    [this]()
                    {
                        const auto* PC = World->GetFirstPlayerController();
                        TestNotNull("PlayerController must exist", PC);

                        const auto* Pawn = PC->GetPawn<ASG_Pawn>();
                        TestNotNull("PlayerPawn must be setup correctly", Pawn);

                        const auto* GameMode = World->GetAuthGameMode<ASG_GameMode>();
                        TestNotNull("GameMode must be setup correctly", GameMode);
                    });
             });
}

#endif
