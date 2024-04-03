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
					CanvasSlot->SetSize(FVector2D(200.0f, 50.0f));
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Posizione"));
				}			
				FString MoveString = MoveReference->AlgebricMoveNotation();
				MoveBox->MoveNotation->SetText(FText::FromString(MoveString));
				AllMoves.Add(MoveBox);
			}
		}
	};
}