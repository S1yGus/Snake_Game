// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

namespace SnakeGame
{

class Grid;

class Game
{
public:
    Game(const Settings& setiings);

    /**
     * Returns the pointer to the grid object
     * @return TSharedPtr<Grid> Shared pointer to the grid object
     */
    TSharedPtr<Grid> grid() const { return m_grid; }

private:
    const Settings c_settings;
    TSharedPtr<Grid> m_grid;
};

}    // namespace SnakeGame
