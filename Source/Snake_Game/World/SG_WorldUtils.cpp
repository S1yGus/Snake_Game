// Snake_Game, all rights reserved.

#include "World/SG_WorldUtils.h"
#include "InputMappingContext.h"
#include "InputAction.h"

using namespace SnakeGame;

FVector Utils::PosToVector(const Position& Position, const Dim& GridSize, uint32 CellSize)
{
    return FVector((GridSize.height - 1 - Position.y) * CellSize, Position.x * CellSize, 0.0) + FVector(CellSize * 0.5);
}

FRotator Utils::InputToRotator(const Input& Input)
{
    double Yaw{0.0};
    if (Input.y > 0)
        Yaw = 90.0;
    else if (Input.y < 0)
        Yaw = -90.0;
    else if (Input.x < 0)
        Yaw = 180.0;

    return {0.0, Yaw, 0.0};
}

FText Utils::FormatScore(uint32 Score)
{
    return FText::FromString(FString::Printf(TEXT("%03d"), Score));
}

FText Utils::FormatTime(float Seconds)
{
    FTimespan Time{FTimespan::FromSeconds(Seconds)};
    return FText::FromString(FString::Printf(TEXT("%02d:%02d"), Time.GetMinutes(), Time.GetSeconds()));
}

FString Utils::GetActionKeyName(TObjectPtr<const UInputMappingContext> MappingContext, TObjectPtr<const UInputAction> Action)
{
    auto* FoundMapping = MappingContext->GetMappings().FindByPredicate(
        [&](const FEnhancedActionKeyMapping& ActionKeyMapping)
        {
            return ActionKeyMapping.Action == Action;
        });

    return FoundMapping ? FoundMapping->Key.GetFName().ToString() : "";
}

void Utils::SetUIInput(const UWorld* World, bool bEnabled)
{
    if (auto* PC = World ? World->GetFirstPlayerController() : nullptr)
    {
        PC->bShowMouseCursor = bEnabled;
        bEnabled ? PC->SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false)) : PC->SetInputMode(FInputModeGameOnly());
    }
}
