// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SG_BaseCellObject.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;

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
    virtual void RestartScaling();

    /**
     * Reproduces the teardown effect
     */
    virtual void Teardown();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USceneComponent> Origin;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName MaterialColorParameterName{"Color"};

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0.0"))
    float ScaleInterpSpeed{1.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Settings", Meta = (ClampMin = "0.0"))
    float CellSizeFactor{1.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<UNiagaraSystem> TeardownEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    FName EffectColorParameterName{"Color"};

    virtual void BeginPlay() override;

private:
    FVector TargetScale;
    FTimerHandle ScaleTimerHandle;
    FLinearColor ObjectColor{FLinearColor::Black};

    void OnChangingScale();
    virtual void OnScalingDone();
};
