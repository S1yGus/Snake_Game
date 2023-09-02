// Snake_Game, all rights reserved.

#include "UI/SG_MenuHUD.h"
#include "UI/SG_MenuWidget.h"

void ASG_MenuHUD::BeginPlay()
{
    Super::BeginPlay();

    auto* MenuWidget = CreateWidget<USG_MenuWidget>(GetWorld(), MenuWidgetClass);
    check(MenuWidget);
    MenuWidget->AddToViewport();
}
