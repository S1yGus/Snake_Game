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
    void RestartScaling();

    /**
     * Reproduces the teardown effect
     */
    virtual void Teardown();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FName MaterialColorParameterName{"Color"};

    UPROPERTY(EditDefaultsOnly, Category = "Settings")
    float ScaleInterpSpeed{1.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UNiagaraSystem* TeardownEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    FName EffectColorParameterName{"Color"};

    virtual void BeginPlay() override;

private:
    FVector TargetScale;
    FTimerHandle ScaleTimerHandle;
    FLinearColor ObjectColor;

    void OnChangingScale();
};
