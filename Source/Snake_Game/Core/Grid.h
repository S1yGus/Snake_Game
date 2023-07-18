// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

namespace Snake
{

class Grid
{
public:
    Grid(const Dim& size);

    const Dim& size() const { return c_size; }

private:
    const Dim c_size;
    TArray<CellType> m_cells;

    void initWalls();
    void printDebug();
};

}    // namespace Snake
