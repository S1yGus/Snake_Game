// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_BaseGameWidget.generated.h"

class UTextBlock;

UCLASS()
class SNAKE_GAME_API USG_BaseGameWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetScore(uint32 Score);
    void SetResetKeyName(const FString& ResetKeyName);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ScoreText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ResetGameText;
};
