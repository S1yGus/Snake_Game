// Snake_Game, all rights reserved.

#include "World/SG_Snake.h"
#include "Core/CoreTypes.h"
#include "Core/Snake.h"
#include "World/SG_WorldTypes.h"
#include "World/SG_SnakeLink.h"

DEFINE_LOG_CATEGORY_STATIC(LogSnakeView, All, All)

using namespace SnakeGame;

ASG_Snake::ASG_Snake()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Snake::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!CoreSnake.IsValid())
        return;

    auto* LinkModel = CoreSnake.Pin()->links().GetHead();
    for (auto* LinkView : Links)
    {
        LinkView->SetActorLocation(GetActorLocation() + PosToVector(LinkModel->GetValue()));
        LinkModel = LinkModel->GetNextNode();
    }
}

void ASG_Snake::SetModel(const TSharedPtr<Snake>& Snake, uint32 InCellSize, const Dim& InGridSize)
{
    if (!Snake.IsValid())
    {
        UE_LOG(LogSnakeView, Fatal, TEXT("Snake model is nullptr, game aborted!"));
    }

    CoreSnake = Snake;
    CellSize = InCellSize;
    GridSize = InGridSize;
}

void ASG_Snake::UpdateColors(const FSnakeColorsTableRow& ColorsSet)
{
    uint32 i{0};
    for (auto* Link : Links)
    {
        const bool bIsHead = i++ == 0;
        Link->UpdateColor(bIsHead ? ColorsSet.SnakeHeadColor : ColorsSet.SnakeBodyColor);
    }
}

void ASG_Snake::BeginPlay()
{
    Super::BeginPlay();

    if (!CoreSnake.IsValid() || !GetWorld())
        return;

    uint32 i{0};
    for (const auto& LinkPosition : CoreSnake.Pin()->links())
    {
        const bool bIsHead = i++ == 0;
        const auto SpawnTransform = GetActorTransform() * FTransform{PosToVector(LinkPosition)};
        auto* Link = GetWorld()->SpawnActorDeferred<ASG_SnakeLink>(bIsHead ? SnakeHeadClass : SnakeBodyClass, SpawnTransform);
        Link->UpdateScale(CellSize);
        Link->FinishSpawning(SpawnTransform);
        Links.Add(Link);
    }
}

FVector ASG_Snake::PosToVector(const Position& Position)
{
    return FVector((GridSize.height - 1 - Position.y) * CellSize, Position.x * CellSize, 0.0) + FVector(CellSize * 0.5);
}
