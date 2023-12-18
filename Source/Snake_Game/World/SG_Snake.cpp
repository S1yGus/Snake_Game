// Snake_Game, all rights reserved.

#include "World/SG_Snake.h"
#include "Core/CoreTypes.h"
#include "Core/Snake.h"
#include "World/SG_WorldTypes.h"
#include "World/SG_SnakeLink.h"
#include "World/SG_WorldUtils.h"
#include "World/SG_ObjectPool.h"

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
    for (auto LinkView : Links)
    {
        const auto NewLocation = GetActorLocation() + Utils::PosToVector(LinkModel->GetValue(), GridSize, CellSize);
        LinkView->SetActorLocation(NewLocation);
        LinkModel = LinkModel->GetNextNode();
    }

    while (LinkModel)
    {
        const auto SpawnTransform = GetActorTransform() * FTransform{Utils::PosToVector(LinkModel->GetValue(), GridSize, CellSize)};
        auto* Link = LinkPool->Pop(GetWorld(), SnakeLinkClass, SpawnTransform);
        Link->UpdateScale(CellSize);
        Link->UpdateColor(LinkColor);
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

    InitPool();
    EmptyLinks();
    SpwnLinks();
}

void ASG_Snake::UpdateColors(const FSnakeColorsTableRow& ColorsSet)
{
    uint32 i{0};
    for (auto Link : Links)
    {
        const bool bIsHead = i++ == 0;
        Link->UpdateColor(bIsHead ? ColorsSet.SnakeHeadColor : ColorsSet.SnakeBodyColor);
    }

    LinkColor = ColorsSet.SnakeBodyColor;
}

void ASG_Snake::Teardown()
{
    EmptyLinks();
    CoreSnake.Reset();
}

void ASG_Snake::InitPool()
{
    if (!LinkPool)
    {
        LinkPool = NewObject<USG_ObjectPool>();
        LinkPool->Reserve(GetWorld(), SnakeLinkClass, ReservedLinksNumber);
    }
}

void ASG_Snake::EmptyLinks()
{
    for (auto Link : Links)
    {
        LinkPool->Add(Link);
    }
    Links.Empty();
}

void ASG_Snake::SpwnLinks()
{
    if (!GetWorld())
        return;

    for (const auto& LinkPosition : CoreSnake.Pin()->links())
    {
        const auto SpawnTransform = GetActorTransform() * FTransform{Utils::PosToVector(LinkPosition, GridSize, CellSize)};
        auto* Link = LinkPool->Pop(GetWorld(), SnakeLinkClass, SpawnTransform);
        Link->UpdateScale(CellSize);
        Links.Add(Link);
    }
}
