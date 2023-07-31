#include "World/SG_WorldUtils.h"

using namespace SnakeGame;

FVector Utils::PosToVector(const Position& Position, const Dim& GridSize, uint32 CellSize)
{
    return FVector((GridSize.height - 1 - Position.y) * CellSize, Position.x * CellSize, 0.0) + FVector(CellSize * 0.5);
}
