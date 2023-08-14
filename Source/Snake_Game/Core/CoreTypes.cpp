// Snake_Game, all rights reserved.

#include "Core/CoreTypes.h"

using namespace SnakeGame;

const Input Input::defaultInput = {1, 0};

const Position Position::zero = {0, 0};

TOptional<Position> PositionRandomizer::randomEmptyPosition(const TArray<CellType>& cells, uint32 width) const
{
    const auto index = FMath::RandHelper(cells.Num());
    for (uint32 i = index; i != cells.Num(); ++i)
    {
        if (cells[i] == CellType::Empty)
        {
            return indexToPos(i, width);
        }
    }

    for (uint32 i = 0; i != index; ++i)
    {
        if (cells[i] == CellType::Empty)
        {
            return indexToPos(i, width);
        }
    }

    return NullOpt;
}

Position PositionRandomizer::indexToPos(uint32 index, uint32 width) const
{
    return {index % width, static_cast<uint32>(index / width)};
}
