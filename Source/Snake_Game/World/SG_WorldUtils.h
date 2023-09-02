// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

class UInputMappingContext;
class UInputAction;

namespace SnakeGame
{

class Utils
{
public:
    static FVector PosToVector(const Position& Position, const Dim& GridSize, uint32 CellSize);
    static FText FormatScore(uint32 Score);
    static FText FormatTime(float Seconds);
    static FString GetActionKeyName(TObjectPtr<const UInputMappingContext> MappingContext, TObjectPtr<const UInputAction> Action);
    static void SetUIInput(const UWorld* World, bool bEnabled);
};

}    // namespace SnakeGame
