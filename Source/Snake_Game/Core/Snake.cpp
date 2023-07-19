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
    m_links.RemoveNode(m_links.GetTail());
    m_links.AddHead(m_links.GetHead()->GetValue() + input);
}
