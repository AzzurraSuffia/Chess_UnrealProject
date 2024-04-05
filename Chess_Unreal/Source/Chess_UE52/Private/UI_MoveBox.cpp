// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MoveBox.h"

void UUI_MoveBox::NativeConstruct()
{
	Super::NativeConstruct();

	ExternalBox = Cast<USizeBox>(GetWidgetFromName(TEXT("Box")));
	Button = Cast<UButton>(GetWidgetFromName(TEXT("MoveButton")));
	MoveNotation = Cast<UTextBlock>(GetWidgetFromName(TEXT("MoveString")));
	if (IsValid(ExternalBox) && IsValid(Button) && IsValid(MoveNotation))
	{
		ExternalBox->SetWidthOverride(700.0f);
		ExternalBox->SetHeightOverride(100.0f);
		MoveNotation->SetText(FText::FromString("Move"));
		Button->OnClicked.AddDynamic(this, &UUI_MoveBox::OnButtonClicked);
	}
}

void UUI_MoveBox::OnButtonClicked()
{

	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

    int32 ClickedMoveIdx = GameMode->ChessBoard->MoveStack.Find(Move);
    int32 CurrentMoveIdx = -1;
    TArray<ATile*> PreviousColoredTiles = {};
   
    if (GameMode->ChessBoard->CurrentChessboardState != nullptr)
    {
        CurrentMoveIdx = GameMode->ChessBoard->MoveStack.Find(GameMode->ChessBoard->CurrentChessboardState);
        PreviousColoredTiles = { GameMode->ChessBoard->CurrentChessboardState->To,  GameMode->ChessBoard->CurrentChessboardState->From };
    }
    else
    {
        CurrentMoveIdx = GameMode->ChessBoard->MoveStack.Find(GameMode->ChessBoard->MoveStack.Last());
        PreviousColoredTiles = { GameMode->ChessBoard->MoveStack.Last()->To,  GameMode->ChessBoard->MoveStack.Last()->From };
    }

    if (ClickedMoveIdx != INDEX_NONE && CurrentMoveIdx != INDEX_NONE)
    {
        GameMode->ChessBoard->RestoreSquareColor(PreviousColoredTiles);

        Move->To->SetTileColor(3);
        Move->From->SetTileColor(3);

        if (ClickedMoveIdx < CurrentMoveIdx)
        {
            // ClickedMove is placed before CurrentMove in the stack
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Cliccata Mossa Precedente"));
            for (int32 i = CurrentMoveIdx; i > ClickedMoveIdx; i--)
            {
                GameMode->ChessBoard->MoveStack[i]->UndoMove(GameMode);
            }
        }
        else
        {
            // ClickedMove is placed after CurrentMove in the stack
            for (int32 i = CurrentMoveIdx + 1; i <= ClickedMoveIdx; i++)
            {
                GameMode->ChessBoard->MoveStack[i]->doMove(GameMode);
            }
        }

        GameMode->ChessBoard->CurrentChessboardState = Move;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("One of the moves is not in the MoveStack"));
    }
}
