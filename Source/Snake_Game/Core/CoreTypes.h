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
    Snake
};

struct Dim
{
    uint32 width;
    uint32 height;
};

struct Input
{
    int8 x;
    int8 y;
};

struct Position
{
    uint32 x;
    uint32 y;

    FORCEINLINE Position operator+(const Input& input) { return {x + input.x, y + input.y}; }
};

struct Settings
{
    Dim gridSize;
    float gameSpeed;
    struct Snake
    {
        uint32 defaultSize;
        Position startPosition;
    } snake;
};

using SnakeList = TDoubleLinkedList<Position>;
using SnakeNode = SnakeList::TDoubleLinkedListNode;

}    // namespace SnakeGame
