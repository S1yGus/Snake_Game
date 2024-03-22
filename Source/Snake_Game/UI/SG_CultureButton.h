// Snake_Game, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Framework/SG_GameUserSettings.h"
#include "SG_CultureButton.generated.h"

DECLARE_DELEGATE_OneParam(FOnClickedCultureButtonSignature, ECulture);

class UImage;
class UButton;

UCLASS()
class SNAKE_GAME_API USG_CultureButton : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnClickedCultureButtonSignature OnClickedCultureButton;

    void Init(const FCultureData& CultureData);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void SetButtonSize(FVector2D Size);

protected:
    UPROPERTY(Meta = (BindWidget))
    UImage* RepresentationImage;

    UPROPERTY(Meta = (BindWidget))
    UButton* CultureSelectButton;

    UPROPERTY(EditDefaultsOnly)
    FLinearColor HoveredTint{1.0f, 1.0f, 1.0f, 0.5f};

    virtual void NativeOnInitialized() override;

private:
    ECulture Culture;

    UFUNCTION()
    void OnClicked();
    UFUNCTION()
    void OnHovered();
    UFUNCTION()
    void OnUnhovered();
};
