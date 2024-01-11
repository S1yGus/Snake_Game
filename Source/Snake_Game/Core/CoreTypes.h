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

FORCEINLINE Position indexToPos(uint32 index, uint32 width)
{
    return {index % width, static_cast<uint32>(index / width)};
}

FORCEINLINE uint32 posToIndex(uint32 x, uint32 y, uint32 width)
{
    return y * width + x;
}

FORCEINLINE uint32 posToIndex(const Position& pos, uint32 width)
{
    return posToIndex(pos.x, pos.y, width);
}

class SNAKE_GAME_API IPositionRandomizer
{
public:
    virtual ~IPositionRandomizer() = default;
    virtual TOptional<Position> randomEmptyPosition(const TArray<CellType>& cells, const Dim& size) const = 0;
};

class SNAKE_GAME_API PositionRandomizer : public IPositionRandomizer
{
public:
    virtual TOptional<Position> randomEmptyPosition(const TArray<CellType>& cells, const Dim& size) const override;
};

struct SNAKE_GAME_API Settings
{
    Dim gridSize;
    float gameSpeed;
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
