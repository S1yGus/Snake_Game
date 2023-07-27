// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

namespace SnakeGame
{

class Food
{
public:
    Food() = default;

    /**
     * Sets food position
     * @param position Position on the grid
     */
    void setPosition(const Position& position);

    /**
     * Returns food position
     * @return Position Food position on the grid
     */
    const Position& position() const { return m_position; }

private:
    Position m_position;
};

}    // namespace SnakeGame
