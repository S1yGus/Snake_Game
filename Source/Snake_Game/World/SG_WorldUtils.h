#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

namespace SnakeGame
{

class Utils
{
public:
    static FVector PosToVector(const Position& Position, const Dim& GridSize, uint32 CellSize);
};

}    // namespace SnakeGame
