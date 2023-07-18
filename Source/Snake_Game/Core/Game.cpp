// Snake_Game, all rights reserved.

#include "Core/Game.h"
#include "Core/Grid.h"

using namespace Snake;

Game::Game(const Settings& setiings) : c_settings{setiings}
{
    m_grid = MakeShared<Grid>(setiings.gridSize);
    check(m_grid.IsValid())
}
