// Snake_Game, all rights reserved.

#include "World/SG_SnakeLink.h"
#include "Components/StaticMeshComponent.h"

static constexpr float ScalingTimerRate = 0.016f;

ASG_SnakeLink::ASG_SnakeLink()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    LinkMesh = CreateDefaultSubobject<UStaticMeshComponent>("LinkMesh");
    check(LinkMesh);
    LinkMesh->SetupAttachment(GetRootComponent());
}

void ASG_SnakeLink::UpdateScale(uint32 CellSize)
{
    check(LinkMesh->GetStaticMesh());
    const FBox Box = LinkMesh->GetStaticMesh()->GetBoundingBox();
    const FVector BoxSize = Box.GetSize();
    check(BoxSize.X);
    check(BoxSize.Y);
    check(BoxSize.Z);
    TargetScale = FVector(CellSize / BoxSize.X, CellSize / BoxSize.Y, CellSize / BoxSize.Z);
}

void ASG_SnakeLink::UpdateColor(const FLinearColor& Color)
{
    if (auto* Material = LinkMesh->CreateAndSetMaterialInstanceDynamic(0))
    {
        Material->SetVectorParameterValue(ColorParameterName, Color);
    }
}

void ASG_SnakeLink::BeginPlay()
{
    Super::BeginPlay();

    LinkMesh->SetRelativeScale3D(FVector::ZeroVector);
    GetWorldTimerManager().SetTimer(ScaleTimerHandle, this, &ThisClass::OnChangingScale, ScalingTimerRate, true);
}

void ASG_SnakeLink::OnChangingScale()
{
    LinkMesh->SetRelativeScale3D(FMath::VInterpConstantTo(LinkMesh->GetRelativeScale3D(), TargetScale, ScalingTimerRate, ScaleInterpSpeed));

    if (FMath::IsNearlyEqual(LinkMesh->GetRelativeScale3D().X, TargetScale.X)        //
        && FMath::IsNearlyEqual(LinkMesh->GetRelativeScale3D().Y, TargetScale.Y)     //
        && FMath::IsNearlyEqual(LinkMesh->GetRelativeScale3D().Z, TargetScale.Z))    //
    {
        LinkMesh->SetRelativeScale3D(TargetScale);
        GetWorldTimerManager().ClearTimer(ScaleTimerHandle);
    }
}
