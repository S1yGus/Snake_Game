// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

namespace SnakeGame
{

class Snake
{
public:
    Snake(const Settings::Snake& settings);

    /**
     * Returns list of all snake links
     * @return SnakeList Links
     */
    const SnakeList& links() const { return m_links; }

    /**
     * Returns snake head position
     * @return Position Head position
     */
    const Position& head() const { return m_links.GetHead()->GetValue(); }

    /**
     * Returns pointer to the body position link (without head)
     * @return SnakeNode Pointer to the body position link
     */
    const SnakeNode* body() const { return m_links.GetHead()->GetNextNode(); }

    /**
     * Moves snake links with a given input
     * @param input User input
     */
    void move(const Input& input);

private:
    SnakeList m_links;
};

}    // namespace SnakeGame