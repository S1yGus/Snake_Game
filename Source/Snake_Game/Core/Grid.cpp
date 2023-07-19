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

void Grid::initWalls()
{
    for (uint32 y = 0; y != c_size.height; ++y)
    {
        for (uint32 x = 0; x != c_size.width; ++x)
        {
            if (y == 0 || y == c_size.height - 1 || x == 0 || x == c_size.width - 1)
            {
                m_cells[x + y * c_size.width] = CellType::Wall;
            }
        }
    }
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
            switch (m_cells[x + y * c_size.width])
            {
                case CellType::Empty:
                    ch = '0';
                    break;
                case CellType::Wall:
                    ch = '*';
                    break;
            }

            line.AppendChar(ch).AppendChar(' ');
        }

        UE_LOG(LogGrid, Display, TEXT("%s"), *line);
    }
#endif
}
