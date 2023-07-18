// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

namespace Snake
{

class Grid;

class Game
{
public:
    Game(const Settings& setiings);

    TSharedPtr<Grid> grid() const { return m_grid; }

private:
    const Settings c_settings;
    TSharedPtr<Grid> m_grid;
};

}    // namespace Snake
