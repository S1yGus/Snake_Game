// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/CoreTypes.h"

namespace SnakeGame
{

class Grid;
class Snake;
class Food;

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
     * Returns the pointer to the food object
     * @return TSharedPtr<Snake> Shared pointer to the food object
     */
    TSharedPtr<Food> food() const { return m_food; }

    /**
     * Updates game
     * @param deltaSeconds Delta time
     * @param input Current user input
     */
    void update(float deltaSeconds, const Input& input);

    /**
     * Returns the current game score
     * @return uint32 Current game score
     */
    uint32 score() const { return m_score; }

    /**
     * Returns the current game time
     * @return float Current game time
     */
    float gameTime() const { return m_gameTime; }

    /**
     * Sets the game event callback function
     * @param callback Callback function
     */
    void subscribeOnGameEvent(const GameEventCallback& callback);

private:
    const Settings c_settings;
    TSharedPtr<Grid> m_grid;
    TSharedPtr<Snake> m_snake;
    TSharedPtr<Food> m_food;
    float m_gameTime{0};
    float m_pastSeconds{0};
    bool m_gameOver{false};
    uint32 m_score{0};
    TArray<GameEventCallback> m_gameEventCallbacks;

    void moveSnake(const Input& input);
    bool updateTime(float deltaSeconds);
    bool died() const;
    bool foodTaken() const;
    void generateFood();
    void dispatchGameEvent(GameEvent event);
};

}    // namespace SnakeGame
