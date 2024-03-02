// Snake_Game, all rights reserved.

#include "SG_WorldTypes.h"

FLinearColor FFruitData::GetRandomColor() const
{
    if (!Colors.IsEmpty())
    {
        return Colors[FMath::RandHelper(Colors.Num())];
    }
    return FLinearColor::Black;
}
