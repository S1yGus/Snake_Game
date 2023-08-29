// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SG_BaseGameWidget.h"
#include "SG_GameWidget.generated.h"

UCLASS()
class SNAKE_GAME_API USG_GameWidget : public USG_BaseGameWidget
{
    GENERATED_BODY()
public:
    void SetTime(float Seconds);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TimeText;
};
