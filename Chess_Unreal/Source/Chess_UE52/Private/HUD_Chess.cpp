// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_Chess.h"
#include "Components/CanvasPanelSlot.h"

void UHUD_Chess::NativeConstruct()
{
	Super::NativeConstruct();

	ScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox")));
	Canvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel")));
	VisibleZone = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Visible")));

	if (ResetWidget)
	{
		//add the restart button to the viewport
		UUserWidget* ChildWidget = CreateWidget<UUserWidget>(GetWorld(), ResetWidget);
		ResetButtonWidget = Cast<UUI_ResetButton>(ChildWidget);
		if (IsValid(ResetButtonWidget))
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(VisibleZone->AddChildToCanvas(ResetButtonWidget));
			if (IsValid(CanvasSlot))
			{	
				//set its size and position
				CanvasSlot->SetPosition(FVector2D(36.0f, 508.0f));
				CanvasSlot->SetSize(FVector2D(270.0f, 40.0f));
			}
		}
	}
}

UUI_MoveBox* UHUD_Chess::AddMoveWidget(UMove* MoveReference)
{
	//if a white move has to be added, add also the move number as a UI_TextBox before
	if (MoveReference->PieceMoving->PieceColor == EColor::WHITE)
	{
		FString NumberOfMove = FString::FromInt(MoveReference->MoveNumber);
		NumberOfMove.Append(". ");
		float Xposition = 0.0f;
		float Yposition = (MoveReference->MoveNumber - 1) * 50.0f;
		AddTextWidget(NumberOfMove, FVector2D(Xposition, Yposition), FVector2D(50.0f, 50.0f));
	}

	//create a move button widget
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
					if (MoveReference->MoveNumber < 100)
					{
						//set its position based on the move number (y)
						float Yposition = (MoveReference->MoveNumber - 1) * 50.0f;

						// and on the player (x)
						float Xposition = 50.0f;
						if (MoveReference->PieceMoving->PieceColor == EColor::BLACK)
						{
							Xposition = 280.0f;
						}
						CanvasSlot->SetPosition(FVector2D(Xposition, Yposition));
						CanvasSlot->SetSize(FVector2D(230.0f, 50.0f));
					}
					//if the move number is equal or over 100 shrink button width
					else
					{
						//set its position based on the move number (y)
						float Yposition = (MoveReference->MoveNumber - 1) * 50.0f;

						// and on the player (x)
						float Xposition = 70.0f;
						if (MoveReference->PieceMoving->PieceColor == EColor::BLACK)
						{
							Xposition = 290.0f;
						}
						CanvasSlot->SetPosition(FVector2D(Xposition, Yposition));
						CanvasSlot->SetSize(FVector2D(220.0f, 50.0f));
					}
				}		

				//add the Long Algebraic Notation of the move as text in the button
				FString MoveString = MoveReference->AlgebricMoveNotation();
				MoveBox->MoveNotation->SetText(FText::FromString(MoveString));

				//add it to the array of move buttons in storyboard
				AllMoves.Add(MoveBox);
				return MoveBox;
			}
		}
	};
	return nullptr;
}

void UHUD_Chess::AddTextWidget(FString& StringName, FVector2D Position, FVector2D Size)
{
	//create a text widget
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
					//set its size and position based on the arguments
					CanvasTextSlot->SetPosition(Position);
					CanvasTextSlot->SetSize(Size);
				}

				if (TextBox->Text != nullptr)
				{
					//set the text in text block based on the arguments
					TextBox->Text->SetText(FText::FromString(StringName));
				} 

				//add it to the array of text boxes in storyboard
				OtherNotationComponents.Add(TextBox);
			}
		}
	}
}
