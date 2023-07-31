// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SG_BaseCellObject.generated.h"

class UStaticMeshComponent;

UCLASS()
class SNAKE_GAME_API ASG_BaseCellObject : public AActor
{
    GENERATED_BODY()

public:
    ASG_BaseCellObject();

    /**
     * Updates cell object target scale
     * @param CellSize World size of the cell
     */
    void UpdateScale(uint32 CellSize);

    /**
     * Updates cell object color
     * @param Color Cell object base color
     */
    void UpdateColor(const FLinearColor& Color);

    /**
     * Restarts the scale of the object from zero to the target scale
     */
    void RestartScaling();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName ColorParameterName{"Color"};

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    float ScaleInterpSpeed{5.0f};

    virtual void BeginPlay() override;

private:
    FVector TargetScale;
    FTimerHandle ScaleTimerHandle;

    void OnChangingScale();
};
