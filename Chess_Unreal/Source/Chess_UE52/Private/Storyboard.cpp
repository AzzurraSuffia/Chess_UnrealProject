// Fill out your copyright notice in the Description page of Project Settings.

#include "Storyboard.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"


void UStoryboard::NativeConstruct()
{
	Super::NativeConstruct();
	ScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("MovesScrollBox")));
	Canvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasStoryboard")));
	SizeBox = Cast<USizeBox>(GetWidgetFromName(TEXT("SizeBoxStoryboard")));
	TextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlockStoryboard")));
	if (IsValid(ScrollBox) && IsValid(Canvas) && IsValid(SizeBox) && IsValid(TextBlock))
	{
		ScrollBox->SetRenderTranslation(FVector2D(1500.0f, 100.0f));
		SizeBox->SetWidthOverride(200.0f);
		SizeBox->SetHeightOverride(50.0f);
		TextBlock->SetText(FText::FromString("Storyboard"));
	}
}

void UStoryboard::AddMoveWidget(UMove* MoveReference)
{
	if (ChildMoveWidget)
	{
		UUserWidget* ChildWidget = CreateWidget<UUserWidget>(GetWorld(), ChildMoveWidget);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Widget Creato"));
		if (ChildWidget)
		{
			UUI_MoveBox* MoveBox = Cast<UUI_MoveBox>(ChildWidget);
			if (IsValid(MoveBox))
			{
				UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas->AddChildToCanvas(MoveBox));
				MoveBox->ExternalBox->SetHeightOverride(100.0f);
				MoveBox->ExternalBox->SetWidthOverride(400.0f);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Altezza e larghezza"));
				if (IsValid(CanvasSlot))
				{
					float Yposition = MoveReference->MoveNumber * 50.0f + 50.0f;
					float Xposition = 0.0f;
					if (MoveReference->PieceMoving->PieceColor == EColor::BLACK)
					{
						Xposition = 200.0f;
					}
					CanvasSlot->SetPosition(FVector2D(Xposition, Yposition));
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Posizione"));
					FString MoveString = MoveReference->AlgebricMoveNotation();
					MoveBox->MoveNotation->SetText(FText::FromString(MoveString));
				}
				FString MoveString = MoveReference->AlgebricMoveNotation();
				MoveBox->MoveNotation->SetText(FText::FromString(MoveString));
				AllMoves.Add(MoveBox);
			}
		}
	};
}