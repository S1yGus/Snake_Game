// Snake_Game, all rights reserved.

#include "Core/CoreTypes.h"

using namespace SnakeGame;

const Input Input::defaultInput = {1, 0};

const Position Position::zero = {0, 0};

TOptional<Position> PositionRandomizer::randomEmptyPosition(const TArray<CellType>& cells, const Dim& size) const
{
    const Position randomStart{.x = static_cast<uint32>(FMath::RandRange(1, size.width - 2)),    //
                               .y = static_cast<uint32>(FMath::RandRange(1, size.height - 2))};
    Position randomPosition{randomStart};
    do
    {
        if (cells[posToIndex(randomPosition, size.width)] == CellType::Empty)
        {
            return randomPosition;
        }

        if (++randomPosition.x == size.width - 1)
        {
            randomPosition.x = 1;
            if (++randomPosition.y == size.height - 1)
            {
                randomPosition.y = 1;
            }
        }
    } while (randomPosition != randomStart);

    return NullOpt;
}
