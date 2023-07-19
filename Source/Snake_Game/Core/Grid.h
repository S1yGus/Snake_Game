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

    /**
     * Update cells with a given cell type
     * @param link Pointer to the position link
     * @param cellType Type of the cell
     */
    void update(const SnakeNode* link, CellType cellType);

    /**
     * Checks cell type in the given position
     * @param position Position on the grid
     * @param cellType Type of the cell
     * @return bool Hit test status
     */
    bool hitTest(const Position& position, CellType cellType);

private:
    const Dim c_size;
    TArray<CellType> m_cells;
    TMap<CellType, TArray<uint32>> m_indexesByType;

    void initWalls();
    void clearCellsByType(CellType type);
    void printDebug();

    FORCEINLINE uint32 posToIndex(uint32 x, uint32 y);
    FORCEINLINE uint32 posToIndex(const Position& position);
};

}    // namespace SnakeGame
