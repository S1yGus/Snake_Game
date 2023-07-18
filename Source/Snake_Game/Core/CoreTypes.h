// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"

namespace Snake
{

enum class CellType
{
    Empty = 0,
    Wall
};

struct Dim
{
    uint32 width;
    uint32 height;
};

struct Settings
{
    Dim gridSize;
};

}    // namespace Snake
