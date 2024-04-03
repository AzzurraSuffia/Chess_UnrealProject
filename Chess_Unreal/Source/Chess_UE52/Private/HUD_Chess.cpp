// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_Chess.h"
#include "Components/CanvasPanelSlot.h"

void UHUD_Chess::NativeConstruct()
{
	Super::NativeConstruct();

	if (StoryboardWidgetClass)
	{
		UUserWidget* StoryboardUserWidget = CreateWidget<UUserWidget>(GetWorld(), StoryboardWidgetClass);
		if (StoryboardUserWidget)
		{
			Storyboard = Cast<UStoryboard>(StoryboardUserWidget);
			CanvasHUD = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Visible")));

			if (IsValid(Storyboard) && IsValid(CanvasHUD))
			{
				CanvasHUD->AddChildToCanvas(Storyboard);
				UCanvasPanelSlot* ScrollBoxSlot = Cast<UCanvasPanelSlot>(Storyboard->ScrollBox->Slot);
				if (IsValid(ScrollBoxSlot))
				{
					ScrollBoxSlot->SetSize(FVector2D(2500.0f, 5000.0f));
				}
			}
		}
	};
}