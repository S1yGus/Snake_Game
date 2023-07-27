// Snake_Game, all rights reserved.

#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"
#include "Core/Food.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All)

using namespace SnakeGame;

Game::Game(const Settings& settings) : c_settings{settings}
{
    m_grid = MakeShared<Grid>(settings.gridSize);
    check(m_grid.IsValid());
    m_snake = MakeShared<Snake>(settings.snake);
    check(m_snake.IsValid());
    m_food = MakeShared<Food>();
    check(m_food.IsValid());

    // Pass the whole snake with the head to avoid possible generation of the food in place of the snake's head
    m_grid->update(m_snake->links().GetHead(), CellType::Snake);
    generateFood();
}

void Game::update(float deltaSeconds, const Input& input)
{
    if (m_gameOver || !updateTime(deltaSeconds))
        return;

    moveSnake(input);

    if (died())
    {
        m_gameOver = true;

#if !UE_BUILD_SHIPPING
        UE_LOG(LogGame, Display, TEXT("Game Over!"));
        UE_LOG(LogGame, Display, TEXT("Score: %d"), m_score);
#endif
        return;
    }

    if (foodTaken())
    {
        ++m_score;
        m_snake->increase();
        generateFood();
    }
}

void Game::moveSnake(const Input& input)
{
    m_snake->move(input);
    m_grid->update(m_snake->body(), CellType::Snake);
}

bool Game::updateTime(float deltaSeconds)
{
    m_pastSeconds += deltaSeconds;
    if (m_pastSeconds >= c_settings.gameSpeed)
    {
        m_pastSeconds = 0.0f;
        return true;
    }

    return false;
}

bool Game::died() const
{
    return m_grid->hitTest(m_snake->head(), CellType::Wall) ||    //
           m_grid->hitTest(m_snake->head(), CellType::Snake);
}

bool Game::foodTaken() const
{
    return m_grid->hitTest(m_snake->head(), CellType::Food);
}

void Game::generateFood()
{
    if (Position newPosition; m_grid->randomEmptyPosition(newPosition))
    {
        m_food->setPosition(newPosition);
        m_grid->update(m_food->position(), CellType::Food);
    }
    else
    {
        m_gameOver = true;

#if !UE_BUILD_SHIPPING
        UE_LOG(LogGame, Display, TEXT("Game Completed!"));
        UE_LOG(LogGame, Display, TEXT("Score: %d"), m_score);
#endif
    }
}
