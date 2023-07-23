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
        const auto index = posToIndex(link->GetValue());
        m_cells[index] = cellType;
        m_indexesByType[cellType].Add(index);
        link = link->GetNextNode();
    }

    printDebug();
}

bool Grid::hitTest(const Position& position, CellType cellType)
{
    return m_cells[posToIndex(position)] == cellType;
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

void Grid::clearCellsByType(CellType type)
{
    for (auto index : m_indexesByType[type])
    {
        m_cells[index] = CellType::Empty;
    }
    m_indexesByType[type].Empty();
}

void Grid::printDebug()
{
#if !UE_BUILD_SHIPPING
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
            }

            line.AppendChar(ch).AppendChar(' ');
        }

        UE_LOG(LogGrid, Display, TEXT("%s"), *line);
    }
#endif
}

uint32 Grid::posToIndex(uint32 x, uint32 y)
{
    return x + y * c_size.width;
}

uint32 Grid::posToIndex(const Position& position)
{
    return posToIndex(position.x, position.y);
}
