// Snake_Game, all rights reserved.

#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All)

using namespace SnakeGame;

Game::Game(const Settings& settings) : c_settings{settings}
{
    m_grid = MakeShared<Grid>(settings.gridSize);
    check(m_grid.IsValid());
    m_snake = MakeShared<Snake>(settings.snake);
    check(m_snake.IsValid());

    updateGrid();
}

void Game::update(float deltaSeconds, const Input& input)
{
    if (m_gameOver || !updateTime(deltaSeconds))
        return;

    moveSnake(input);

    if (died())
    {
        m_gameOver = true;
        UE_LOG(LogGame, Display, TEXT("Game Over!"))
    }
}

void Game::moveSnake(const Input& input)
{
    m_snake->move(input);
    updateGrid();
}

void Game::updateGrid()
{
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
