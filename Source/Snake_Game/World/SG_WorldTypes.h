// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SG_WorldTypes.generated.h"

USTRUCT(BlueprintType)
struct FSnakeDesignTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridBackgroundColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridWallsColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridLinesColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor FogColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor SnakeHeadColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor SnakeBodyColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<TObjectPtr<UStaticMesh>> FruitMeshes;
};
