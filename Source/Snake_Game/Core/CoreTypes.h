// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"

namespace SnakeGame
{

enum class CellType
{
    Empty = 0,
    Wall,
    Snake,
    Food
};

enum class GameEvent
{
    GameOver = 0,
    GameCompleted,
    FoodTaken
};

struct SNAKE_GAME_API Dim
{
    uint32 width;
    uint32 height;

    FORCEINLINE bool operator==(const Dim& rhs) const { return width == rhs.width && height == rhs.height; }
};

struct SNAKE_GAME_API Input
{
    int8 x;
    int8 y;

    static const Input defaultInput;

    FORCEINLINE bool opposite(const Input& rhs) const { return (x != 0 && x == -rhs.x) || (y != 0 && y == -rhs.y); }
};

struct SNAKE_GAME_API Position
{
    uint32 x;
    uint32 y;

    static const Position zero;

    FORCEINLINE Position operator+(const Input& input) const { return {x + input.x, y + input.y}; }
    FORCEINLINE Position operator+(const Position& rhs) const { return {x + rhs.x, y + rhs.y}; }
    FORCEINLINE Position operator-(const Position& rhs) const { return {x - rhs.x, y - rhs.y}; }
    FORCEINLINE bool operator==(const Position& rhs) const { return x == rhs.x && y == rhs.y; }
    FORCEINLINE bool operator!=(const Position& rhs) const { return x != rhs.x || y != rhs.y; }
};

/**
 * Translates the index position in a one-dimensional array into a coordinate position within a two-dimensional array.
 * @param index The index value in a one-dimensional array
 * @param width The number of columns in a two-dimensional array
 * @return Position The position in a two-dimensional array
 */
FORCEINLINE Position indexToPos(uint32 index, uint32 width)
{
    return {index % width, static_cast<uint32>(index / width)};
}

/**
 * Translates the position value along x and y axes in a two-dimensional array into an index position in a one-dimensional array.
 * @param x The position along the x-axis
 * @param y The position along the y-axis
 * @param width The number of columns in a two-dimensional array
 * @return uint32 The index value in a one-dimensional array
 */
FORCEINLINE uint32 posToIndex(uint32 x, uint32 y, uint32 width)
{
    return y * width + x;
}

/**
 * Translates the position value in a two-dimensional array into an index in a one-dimensional array.
 * @param pos The position value in a two-dimensional array
 * @param width The number of columns in a two-dimensional array
 * @return uint32 The index value in a one-dimensional array
 */
FORCEINLINE uint32 posToIndex(const Position& pos, uint32 width)
{
    return posToIndex(pos.x, pos.y, width);
}

class SNAKE_GAME_API IPositionRandomizer
{
public:
    virtual ~IPositionRandomizer() = default;

    /**
     * Generates a random empty position on the game grid
     * @param cells Array containing information about cells on the grid
     * @param size The dimensions of the game grid
     * @return An optional containing a randomly generated empty position if one was found; otherwise, None
     */
    virtual TOptional<Position> randomEmptyPosition(const TArray<CellType>& cells, const Dim& size) const = 0;
};

class SNAKE_GAME_API PositionRandomizer : public IPositionRandomizer
{
public:
    /**
     * Generates a random empty position on the game grid
     * @param cells Array containing information about cells on the grid
     * @param size The dimensions of the game grid
     * @return An optional containing a randomly generated empty position if one was found; otherwise, None
     */
    virtual TOptional<Position> randomEmptyPosition(const TArray<CellType>& cells, const Dim& size) const override;
};

struct SNAKE_GAME_API SpeedData
{
    float initial{0.0f};
    float limit{0.0f};
    float boost{0.0f};

    bool operator==(const SpeedData& other) { return initial == other.initial && limit == other.limit && boost == other.boost; }
};

/**
 * Returns the computed value of the snake's speed boost when picking up food
 * @param speed Game speed data containing initial and limit values
 * @param gridSize The size of the game grid
 * @param sizeFactor A coefficient ranging from 0 to 1 indicating the required percentage of scores earned to reach the speed limit out of the total number of all
 * possible scores
 * @return float The computed value of the game speed boost
 */
[[nodiscard]] FORCEINLINE float computeSpeedBoost(const SpeedData& speed, const Dim& gridSize, float sizeFactor = 0.25f)
{
    check(sizeFactor <= 1.0f);
    const float sizeAmount = gridSize.width * gridSize.height * sizeFactor;
    check(sizeAmount);
    check(speed.initial >= speed.limit);
    return (speed.initial - speed.limit) / sizeAmount;
}

struct SNAKE_GAME_API Settings
{
    Dim gridSize;
    SpeedData speed;
    struct Snake
    {
        uint32 defaultSize;
        Position startPosition;
    } snake;
    TSharedPtr<IPositionRandomizer> positionRandomizer = MakeShared<PositionRandomizer>();
};

using SnakeList = TDoubleLinkedList<Position>;
using SnakeNode = SnakeList::TDoubleLinkedListNode;

using GameEventCallback = TFunction<void(GameEvent)>;

}    // namespace SnakeGame
