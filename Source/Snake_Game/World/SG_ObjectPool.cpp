// Snake_Game, all rights reserved.

#include "World/SG_ObjectPool.h"
#include "World/SG_BaseCellObject.h"

void USG_ObjectPool::Reserve(UWorld* World, TSubclassOf<ASG_BaseCellObject> ObjectClass, int32 Number)
{
    for (int32 i = 0; i < Number; ++i)
    {
        auto* Object = World->SpawnActor<ASG_BaseCellObject>(ObjectClass, FTransform::Identity);
        Object->SetActorHiddenInGame(true);
        Pool.Add(Object);
    }
}

void USG_ObjectPool::Add(ASG_BaseCellObject* Object)
{
    Object->Teardown();
    Pool.Add(Object);
}

ASG_BaseCellObject* USG_ObjectPool::Pop(UWorld* World, TSubclassOf<ASG_BaseCellObject> ObjectClass, const FTransform& Transform)
{
    ASG_BaseCellObject* Object = nullptr;
    if (Pool.IsEmpty())
    {
        Object = World->SpawnActor<ASG_BaseCellObject>(ObjectClass, Transform);
    }
    else
    {
        Object = Pool.Pop();
        Object->SetActorTransform(Transform);
        Object->SetActorHiddenInGame(false);
        Object->RestartScaling();
    }
    return Object;
}
