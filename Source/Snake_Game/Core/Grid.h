// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

namespace SnakeGame
{

class SNAKE_GAME_API Grid
{
public:
    Grid(const Dim& size, const TSharedPtr<IPositionRandomizer>& positionRandomizer = MakeShared<PositionRandomizer>());

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
     * Update cells with a given cell type
     * @param position Position on the grid to be updated
     * @param cellType Type of the cell
     */
    void update(const Position& position, CellType cellType);

    /**
     * Checks cell type in the given position
     * @param position Position on the grid
     * @param cellType Type of the cell
     * @return bool Hit test status
     */
    bool hitTest(const Position& position, CellType cellType) const;

    /**
     * Returns empty position by ref
     * @param[out] position Position on the grid
     * @return bool True if empty position exists (if position doesn't exist then snake body fills whole grid)
     */
    TOptional<Position> randomEmptyPosition() const;

    /**
     * Returns center position in grid (walls included)
     * @param girdSize Grid size in two dimensions
     * @return Position Center of grid
     */
    static Position center(const Dim& girdSize);

private:
    const Dim c_size;
    TSharedPtr<IPositionRandomizer> m_positionRandomizer;
    TArray<CellType> m_cells;
    TMap<CellType, TArray<uint32>> m_indexesByType;

    void initWalls();
    FORCEINLINE void checkCellType(CellType cellType);
    void clearCellsByType(CellType cellType);
    void updateInternal(const Position& position, CellType cellType);
    void printDebug() const;
};

}    // namespace SnakeGame
