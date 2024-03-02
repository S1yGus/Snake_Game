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

    TSharedPtr<Snake> Snake = CoreSnake.Pin();
    if (!Snake.IsValid() || !GetWorld())
        return;

    auto* LinkModel = Snake->links().GetHead();
    if (!LinkModel)
        return;

    const auto HeadPosition = LinkModel->GetValue();
    if (HeadPosition == PrevHeadPosition)
        return;

    PrevHeadPosition = HeadPosition;
    UpdateLinks(LinkModel);
}

void ASG_Snake::SetModel(const TSharedPtr<Snake>& Snake, uint32 InCellSize, const Dim& InGridSize)
{
    if (!Snake.IsValid())
    {
        UE_LOG(LogSnakeView, Fatal, TEXT("Snake model is nullptr, game aborted!"));
    }

    check(SnakeHeadClass);
    check(SnakeLinkClass);

    CoreSnake = Snake;
    CellSize = InCellSize;
    GridSize = InGridSize;

    InitPools();
    EmptyLinks();
    SpawnLinks();
}

void ASG_Snake::UpdateColors(const FSnakeDesignTableRow& DesignSet)
{
    uint32 i{0};
    for (auto Link : Links)
    {
        const bool bIsHead = i++ == 0;
        Link->UpdateColor(bIsHead ? DesignSet.SnakeHeadColor : DesignSet.SnakeBodyColor);
    }

    LinkColor = DesignSet.SnakeBodyColor;
}

void ASG_Snake::Teardown()
{
    EmptyLinks();
    CoreSnake.Reset();
}

void ASG_Snake::InitPools()
{
    if (!LinkPool)
    {
        LinkPool = NewObject<USG_ObjectPool>();
        LinkPool->Reserve(GetWorld(), SnakeLinkClass, ReservedLinksNumber);
    }

    if (!HeadPool)
    {
        HeadPool = NewObject<USG_ObjectPool>();
        HeadPool->Reserve(GetWorld(), SnakeHeadClass, 1);
    }
}

void ASG_Snake::EmptyLinks()
{
    for (int32 i = 0; i < Links.Num(); ++i)
    {
        const bool bIsHead = i == 0;
        (bIsHead ? HeadPool : LinkPool)->Add(Links[i]);
    }
    Links.Empty();
}

void ASG_Snake::SpawnLinks()
{
    TSharedPtr<Snake> Snake = CoreSnake.Pin();
    if (!Snake.IsValid() || !GetWorld())
        return;

    uint32 i{0};
    for (const auto& LinkPosition : Snake->links())
    {
        const auto SpawnTransform = GetActorTransform() * FTransform{Utils::PosToVector(LinkPosition, GridSize, CellSize)};
        const auto Pop = [&](TObjectPtr<USG_ObjectPool> Pool, TSubclassOf<ASG_SnakeLink> LinkClass)
        {
            return Pool->Pop(GetWorld(), LinkClass, SpawnTransform);
        };

        const bool bIsHead = i++ == 0;
        auto* Link = bIsHead ? Pop(HeadPool, SnakeHeadClass) : Pop(LinkPool, SnakeLinkClass);
        Link->UpdateScale(CellSize);
        Links.Add(Link);

        const bool bIsTail = i == Snake->links().Num();
        if (bIsTail)
        {
            PrevTailPosition = LinkPosition;
        }
    }
}

void ASG_Snake::UpdateLinks(SnakeNode* SnakeNode)
{
    for (auto Link : Links)
    {
        const auto CurrentLinkPostion = SnakeNode->GetValue();
        SnakeNode = SnakeNode->GetNextNode();
        const auto PrevLinkPostion = SnakeNode ? SnakeNode->GetValue() : PrevTailPosition;
        if (!SnakeNode)
        {
            PrevTailPosition = CurrentLinkPostion;
        }

        const auto x = static_cast<int32>(CurrentLinkPostion.x) - static_cast<int32>(PrevLinkPostion.x);
        const auto y = static_cast<int32>(CurrentLinkPostion.y) - static_cast<int32>(PrevLinkPostion.y);
        Link->SetActorRotation(Utils::InputToRotator(Input(x, y)));

        const auto NewLocation = GetActorLocation() + Utils::PosToVector(CurrentLinkPostion, GridSize, CellSize);
        Link->SetActorLocation(NewLocation);
    }

    AddLinks(SnakeNode);
}

void ASG_Snake::AddLinks(SnakeNode* SnakeNode)
{
    while (SnakeNode)
    {
        const auto SpawnTransform = GetActorTransform() * FTransform{Utils::PosToVector(SnakeNode->GetValue(), GridSize, CellSize)};
        auto* Link = LinkPool->Pop(GetWorld(), SnakeLinkClass, SpawnTransform);
        Link->UpdateScale(CellSize);
        Link->UpdateColor(LinkColor);
        Links.Add(Link);
        SnakeNode = SnakeNode->GetNextNode();
    }
}
