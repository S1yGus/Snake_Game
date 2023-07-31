// Snake_Game, all rights reserved.

#include "World/SG_BaseCellObject.h"
#include "Components/StaticMeshComponent.h"

static constexpr float ScalingTimerRate = 0.016f;

ASG_BaseCellObject::ASG_BaseCellObject()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    check(Mesh);
    Mesh->SetupAttachment(GetRootComponent());
}

void ASG_BaseCellObject::UpdateScale(uint32 CellSize)
{
    check(Mesh->GetStaticMesh());
    const FBox Box = Mesh->GetStaticMesh()->GetBoundingBox();
    const FVector BoxSize = Box.GetSize();
    check(!BoxSize.IsZero());
    TargetScale = FVector(CellSize) / BoxSize;
}

void ASG_BaseCellObject::UpdateColor(const FLinearColor& Color)
{
    if (auto* Material = Mesh->CreateAndSetMaterialInstanceDynamic(0))
    {
        Material->SetVectorParameterValue(ColorParameterName, Color);
    }
}

void ASG_BaseCellObject::RestartScaling()
{
    Mesh->SetRelativeScale3D(FVector::ZeroVector);
    GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &ThisClass::OnChangingScale, ScalingTimerRate, true);
}

void ASG_BaseCellObject::BeginPlay()
{
    Super::BeginPlay();

    RestartScaling();
}

void ASG_BaseCellObject::OnChangingScale()
{
    Mesh->SetRelativeScale3D(FMath::VInterpConstantTo(Mesh->GetRelativeScale3D(), TargetScale, ScalingTimerRate, ScaleInterpSpeed));

    if (Mesh->GetRelativeScale3D().Equals(TargetScale))
    {
        Mesh->SetRelativeScale3D(TargetScale);
        GetWorldTimerManager().ClearTimer(ScaleTimerHandle);
    }
}
