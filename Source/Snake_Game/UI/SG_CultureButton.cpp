// Snake_Game, all rights reserved.

#include "UI/SG_CultureButton.h"
#include "Components/Button.h"
#include "Components/Image.h"

void USG_CultureButton::Init(const FCultureData& CultureData)
{
    Culture = CultureData.Culture;

    auto* Representation = LoadObject<UTexture2D>(nullptr, *CultureData.RepresentationReference);
    check(Representation);
    RepresentationImage->SetBrushFromTexture(Representation);
    SetButtonSize(FVector2D(Representation->GetSizeX(), Representation->GetSizeY()));
}

void USG_CultureButton::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(RepresentationImage);
    check(CultureSelectButton);

    CultureSelectButton->OnClicked.AddDynamic(this, &ThisClass::OnClicked);
    CultureSelectButton->OnHovered.AddDynamic(this, &ThisClass::OnHovered);
    CultureSelectButton->OnUnhovered.AddDynamic(this, &ThisClass::OnUnhovered);
}

void USG_CultureButton::OnClicked()
{
    OnClickedCultureButton.Execute(Culture);
}

void USG_CultureButton::OnHovered()
{
    RepresentationImage->SetColorAndOpacity(HoveredTint);
}

void USG_CultureButton::OnUnhovered()
{
    RepresentationImage->SetColorAndOpacity(FLinearColor::White);
}
