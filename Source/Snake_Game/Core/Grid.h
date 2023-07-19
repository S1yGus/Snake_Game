// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

namespace SnakeGame
{

class Grid
{
public:
    Grid(const Dim& size);

    /**
     * Returns grid size including walls (width + 2; height + 2)
     * @return Dim Grid size
     */
    const Dim& size() const { return c_size; }

private:
    const Dim c_size;
    TArray<CellType> m_cells;

    void initWalls();
    void printDebug();
};

}    // namespace SnakeGame
