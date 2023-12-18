// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SG_ObjectPool.generated.h"

class ASG_BaseCellObject;

UCLASS()
class SNAKE_GAME_API USG_ObjectPool : public UObject
{
    GENERATED_BODY()

public:
    /**
     * Reserves the specified number of objects in the pool
     * @param World Game world instance
     * @param ObjectClass Class of reserved objects
     * @param Number Number of reserved objects
     */
    void Reserve(UWorld* World, TSubclassOf<ASG_BaseCellObject> ObjectClass, int32 Number);

    /**
     * Adds an object to the pool
     * @param Object Object to add
     */
    void Add(ASG_BaseCellObject* Object);

    /**
     * Retrieves an object from the pool or spawn a new one if there are no objects in the pool
     * @param World Game world instance
     * @param ObjectClass Class of retrieves object
     * @param Transform Transformation of the retrieves object
     */
    ASG_BaseCellObject* Pop(UWorld* World, TSubclassOf<ASG_BaseCellObject> ObjectClass, const FTransform& Transform);

private:
    TArray<ASG_BaseCellObject*> Pool;
};
