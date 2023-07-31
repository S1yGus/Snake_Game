// Snake_Game, all rights reserved.

#include "Core/Snake.h"

using namespace SnakeGame;

Snake::Snake(const Settings::Snake& settings)
{
    for (uint32 i = 0; i != settings.defaultSize; ++i)
    {
        m_links.AddTail({settings.startPosition.x - i, settings.startPosition.y});
    }
}

void Snake::move(const Input& input)
{
    if (!input.opposite(m_lastInput))
    {
        m_lastInput = input;
    }

    if (!b_increase)
    {
        m_links.RemoveNode(m_links.GetTail());
    }
    else
    {
        b_increase = false;
    }

    m_links.AddHead(m_links.GetHead()->GetValue() + m_lastInput);
}

void Snake::increase()
{
    b_increase = true;
}
