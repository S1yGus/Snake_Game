// Snake_Game, all rights reserved.

#include "Core/Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All)

using namespace SnakeGame;

Grid::Grid(const Dim& dim) : c_size{dim.width + 2, dim.height + 2}
{
    m_cells.Init(CellType::Empty, c_size.width * c_size.height);

    initWalls();
    printDebug();
}

void Grid::update(const SnakeNode* link, CellType cellType)
{
    if (!m_indexesByType.Contains(cellType))
    {
        m_indexesByType.Add(cellType, {});
    }

    clearCellsByType(cellType);

    while (link)
    {
        updateInternal(link->GetValue(), cellType);
        link = link->GetNextNode();
    }

#if !UE_BUILD_SHIPPING
    printDebug();
#endif
}

void Grid::update(const Position& position, CellType cellType)
{
    if (!m_indexesByType.Contains(cellType))
    {
        m_indexesByType.Add(cellType, {});
    }

    clearCellsByType(cellType);

    updateInternal(position, cellType);

#if !UE_BUILD_SHIPPING
    printDebug();
#endif
}

bool Grid::hitTest(const Position& position, CellType cellType) const
{
    return m_cells[posToIndex(position)] == cellType;
}

bool Grid::randomEmptyPosition(Position& position) const
{
    const auto index = FMath::RandHelper(m_cells.Num());
    for (uint32 i = index; i != m_cells.Num(); ++i)
    {
        if (m_cells[i] == CellType::Empty)
        {
            position = indexToPos(i);
            return true;
        }
    }

    for (uint32 i = 0; i != index; ++i)
    {
        if (m_cells[i] == CellType::Empty)
        {
            position = indexToPos(i);
            return true;
        }
    }

    return false;
}

Position Grid::center(const Dim& girdSize)
{
    return {girdSize.width / 2 + 1, girdSize.height / 2 + 1};
}

void Grid::initWalls()
{
    for (uint32 y = 0; y != c_size.height; ++y)
    {
        for (uint32 x = 0; x != c_size.width; ++x)
        {
            if (y == 0 || y == c_size.height - 1 || x == 0 || x == c_size.width - 1)
            {
                m_cells[posToIndex(x, y)] = CellType::Wall;
            }
        }
    }
}

void Grid::clearCellsByType(CellType cellType)
{
    for (auto index : m_indexesByType[cellType])
    {
        m_cells[index] = CellType::Empty;
    }
    m_indexesByType[cellType].Empty();
}

void Grid::updateInternal(const Position& position, CellType cellType)
{
    const auto index = posToIndex(position);
    m_cells[index] = cellType;
    m_indexesByType[cellType].Add(index);
}

void Grid::printDebug()
{
    for (uint32 y = 0; y != c_size.height; ++y)
    {
        FString line;
        for (uint32 x = 0; x != c_size.width; ++x)
        {
            TCHAR ch{' '};
            switch (m_cells[posToIndex(x, y)])
            {
                case CellType::Empty:
                    ch = '0';
                    break;
                case CellType::Wall:
                    ch = '*';
                    break;
                case CellType::Snake:
                    ch = '_';
                    break;
                case CellType::Food:
                    ch = '+';
                    break;
            }

            line.AppendChar(ch).AppendChar(' ');
        }

        UE_LOG(LogGrid, Display, TEXT("%s"), *line);
    }
}

uint32 Grid::posToIndex(uint32 x, uint32 y) const
{
    return x + y * c_size.width;
}

uint32 Grid::posToIndex(const Position& position) const
{
    return posToIndex(position.x, position.y);
}

Position Grid::indexToPos(uint32 index) const
{
    return {index % c_size.width, static_cast<uint32>(index / c_size.width)};
}
