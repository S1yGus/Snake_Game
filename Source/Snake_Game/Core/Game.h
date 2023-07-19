// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

namespace SnakeGame
{

class Grid;
class Snake;

class Game
{
public:
    Game(const Settings& settings);

    /**
     * Returns the pointer to the grid object
     * @return TSharedPtr<Grid> Shared pointer to the grid object
     */
    TSharedPtr<Grid> grid() const { return m_grid; }

    /**
     * Returns the pointer to the snake object
     * @return TSharedPtr<Snake> Shared pointer to the snake object
     */
    TSharedPtr<Snake> snake() const { return m_snake; }

    /**
     * Updates game
     * @param deltaSeconds Delta time
     * @param input Current user input
     */
    void update(float deltaSeconds, const Input& input);

private:
    const Settings c_settings;
    TSharedPtr<Grid> m_grid;
    TSharedPtr<Snake> m_snake;
    float m_pastSeconds{0};
    bool m_gameOver{false};

    void moveSnake(const Input& input);
    void updateGrid();
    bool updateTime(float deltaSeconds);
    bool died() const;
};

}    // namespace SnakeGame
