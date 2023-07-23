// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SG_SnakeLink.generated.h"

class UStaticMeshComponent;

UCLASS()
class SNAKE_GAME_API ASG_SnakeLink : public AActor
{
    GENERATED_BODY()

public:
    ASG_SnakeLink();

    /**
     * Updates link target scale
     * @param CellSize World size of the cell
     */
    void UpdateScale(uint32 CellSize);

    /**
     * Updates link color
     * @param Color Link base color
     */
    void UpdateColor(const FLinearColor& Color);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* LinkMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName ColorParameterName{"Color"};

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    float ScaleInterpSpeed{4.0f};

    virtual void BeginPlay() override;

private:
    FVector TargetScale;
    FTimerHandle ScaleTimerHandle;

    void OnChangingScale();
};
