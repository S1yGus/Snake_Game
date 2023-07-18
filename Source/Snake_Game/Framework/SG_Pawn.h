// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/CoreTypes.h"
#include "SG_Pawn.generated.h"

class UCameraComponent;

UCLASS()
class SNAKE_GAME_API ASG_Pawn : public APawn
{
    GENERATED_BODY()

public:
    ASG_Pawn();

    void UpdateLocation(const Snake::Dim& GridSize, uint32 CellSize, const FTransform& InGridOrigin);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UCameraComponent* Camera;

protected:
    double WorldWidth;
    double WorldHeight;
    FTransform GridOrigin;

    void OnViewportResized(FViewport* Viewport, uint32 Value);
};
