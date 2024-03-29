// Snake_Game, all rights reserved.

#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"
#include "Core/Food.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All)

using namespace SnakeGame;

Game::Game(const Settings& settings) : c_settings{settings}, m_currentGameSpeed{settings.speed.initial}
{
    m_grid = MakeShared<Grid>(settings.gridSize, settings.positionRandomizer);
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
        dispatchGameEvent(GameEvent::GameOver);
        return;
    }

    if (foodTaken())
    {
        ++m_score;
        m_currentGameSpeed = FMath::Max(m_currentGameSpeed - c_settings.speed.boost, c_settings.speed.limit);
        m_snake->increase();
        generateFood();
        dispatchGameEvent(GameEvent::FoodTaken);
    }
}

void Game::subscribeOnGameEvent(const GameEventCallback& callback)
{
    m_gameEventCallbacks.Add(callback);
}

void Game::moveSnake(const Input& input)
{
    m_snake->move(input);
    m_grid->update(m_snake->body(), CellType::Snake);
}

bool Game::updateTime(float deltaSeconds)
{
    m_gameTime += deltaSeconds;
    m_pastSeconds += deltaSeconds;
    if (m_pastSeconds >= m_currentGameSpeed)
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
    if (TOptional<Position> newPosition = m_grid->randomEmptyPosition(); newPosition.IsSet())
    {
        m_food->setPosition(*newPosition);
        m_grid->update(m_food->position(), CellType::Food);
    }
    else
    {
        m_gameOver = true;
        dispatchGameEvent(GameEvent::GameCompleted);
    }
}

void Game::dispatchGameEvent(GameEvent event)
{
    for (auto& callback : m_gameEventCallbacks)
    {
        if (callback)
        {
            callback(event);
        }
    }
}
