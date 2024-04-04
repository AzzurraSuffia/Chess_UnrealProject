// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_Chess.h"
#include "Components/CanvasPanelSlot.h"

void UHUD_Chess::NativeConstruct()
{
	Super::NativeConstruct();

	ScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox")));
	Canvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel")));

}

void UHUD_Chess::AddMoveWidget(UMove* MoveReference)
{
	if (MoveReference->PieceMoving->PieceColor == EColor::WHITE)
	{
		FString NumberOfMove = FString::FromInt(MoveReference->MoveNumber);
		NumberOfMove.Append(". ");
		float Xposition = 0.0f;
		float Yposition = (MoveReference->MoveNumber - 1) * 50.0f;
		AddTextWidget(NumberOfMove, FVector2D(Xposition, Yposition), FVector2D(50.0f, 50.0f));
	}

	if (ChildMoveWidget)
	{
		UUserWidget* ChildWidget = CreateWidget<UUserWidget>(GetWorld(), ChildMoveWidget);
		if (ChildWidget)
		{
			UUI_MoveBox* MoveBox = Cast<UUI_MoveBox>(ChildWidget);
			if (IsValid(MoveBox))
			{
				UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas->AddChildToCanvas(MoveBox));
				if (IsValid(CanvasSlot))
				{
					float Yposition = (MoveReference->MoveNumber-1) * 50.0f;
					float Xposition = 50.0f;
					if (MoveReference->PieceMoving->PieceColor == EColor::BLACK)
					{
						Xposition = 225.0f;
					}
					CanvasSlot->SetPosition(FVector2D(Xposition, Yposition));
					CanvasSlot->SetSize(FVector2D(175.0f, 50.0f));
				}			
				FString MoveString = MoveReference->AlgebricMoveNotation();
				MoveBox->MoveNotation->SetText(FText::FromString(MoveString));
				AllMoves.Add(MoveBox);
			}
		}
	};
}

void UHUD_Chess::AddTextWidget(FString& StringName, FVector2D Position, FVector2D Size)
{
	if (ChildTextWidget)
	{
		UUserWidget* TextWidget = CreateWidget<UUserWidget>(GetWorld(), ChildTextWidget);
		if (TextWidget)
		{
			UUI_TextBox* TextBox = Cast<UUI_TextBox>(TextWidget);
			if (IsValid(TextBox))
			{
				UCanvasPanelSlot* CanvasTextSlot = Cast<UCanvasPanelSlot>(Canvas->AddChildToCanvas(TextBox));
				if (IsValid(CanvasTextSlot))
				{
					CanvasTextSlot->SetPosition(Position);
					CanvasTextSlot->SetSize(Size);
				}

				if (TextBox->Text != nullptr)
				{
					TextBox->Text->SetText(FText::FromString(StringName));
				} 

				OtherNotationComponents.Add(TextBox);
			}
		}
	}
}
