// Snake_Game, all rights reserved.

#include "World/SG_Snake.h"
#include "Core/CoreTypes.h"
#include "Core/Snake.h"
#include "World/SG_WorldTypes.h"
#include "World/SG_SnakeLink.h"
#include "World/SG_WorldUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSnakeView, All, All)

using namespace SnakeGame;

ASG_Snake::ASG_Snake()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Snake::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!CoreSnake.IsValid() || !GetWorld())
        return;

    auto* LinkModel = CoreSnake.Pin()->links().GetHead();
    for (auto* LinkView : Links)
    {
        LinkView->SetActorLocation(GetActorLocation() + Utils::PosToVector(LinkModel->GetValue(), GridSize, CellSize));
        LinkModel = LinkModel->GetNextNode();
    }

    while (LinkModel)
    {
        const auto SpawnTransform = GetActorTransform() * FTransform{Utils::PosToVector(LinkModel->GetValue(), GridSize, CellSize)};
        auto* Link = GetWorld()->SpawnActorDeferred<ASG_SnakeLink>(SnakeBodyClass, SpawnTransform);
        Link->UpdateScale(CellSize);
        Link->UpdateColor(LinkColor);
        Link->FinishSpawning(SpawnTransform);
        Links.Add(Link);
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

    EmptyLinks();
    SpwnLinks();
}

void ASG_Snake::UpdateColors(const FSnakeColorsTableRow& ColorsSet)
{
    uint32 i{0};
    for (auto* Link : Links)
    {
        const bool bIsHead = i++ == 0;
        Link->UpdateColor(bIsHead ? ColorsSet.SnakeHeadColor : ColorsSet.SnakeBodyColor);
    }

    LinkColor = ColorsSet.SnakeBodyColor;
}

void ASG_Snake::Teardown()
{
    for (auto* Link : Links)
    {
        Link->Teardown();
    }
}

void ASG_Snake::EmptyLinks()
{
    for (auto* Link : Links)
    {
        Link->Destroy();
    }
    Links.Empty();
}

void ASG_Snake::SpwnLinks()
{
    if (!GetWorld())
        return;

    uint32 i{0};
    for (const auto& LinkPosition : CoreSnake.Pin()->links())
    {
        const bool bIsHead = i++ == 0;
        const auto SpawnTransform = GetActorTransform() * FTransform{Utils::PosToVector(LinkPosition, GridSize, CellSize)};
        auto* Link = GetWorld()->SpawnActorDeferred<ASG_SnakeLink>(bIsHead ? SnakeHeadClass : SnakeBodyClass, SpawnTransform);
        Link->UpdateScale(CellSize);
        Link->FinishSpawning(SpawnTransform);
        Links.Add(Link);
    }
}
