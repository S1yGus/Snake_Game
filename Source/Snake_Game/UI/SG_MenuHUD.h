// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SG_MenuHUD.generated.h"

class USG_MenuWidget;

UCLASS()
class SNAKE_GAME_API ASG_MenuHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USG_MenuWidget> MenuWidgetClass;

    virtual void BeginPlay() override;
};
