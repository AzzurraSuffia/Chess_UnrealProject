// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MoveBox.h"
#include "Chess_PlayerController.h"
#include "Chess_HumanPlayer.h"

void UUI_MoveBox::NativeConstruct()
{
	Super::NativeConstruct();

	ExternalBox = Cast<USizeBox>(GetWidgetFromName(TEXT("Box")));
	Button = Cast<UButton>(GetWidgetFromName(TEXT("MoveButton")));
	MoveNotation = Cast<UTextBlock>(GetWidgetFromName(TEXT("MoveString")));
	if (IsValid(ExternalBox) && IsValid(Button) && IsValid(MoveNotation))
	{
        //set width and height of the size box
		ExternalBox->SetWidthOverride(700.0f);
		ExternalBox->SetHeightOverride(100.0f);

        //set a default text for the text block
		MoveNotation->SetText(FText::FromString("Move"));

        //add on clicked event
		Button->OnClicked.AddDynamic(this, &UUI_MoveBox::OnButtonClicked);
	}
}

/*
* When the button is clicked, change the chessboard state.
*/
void UUI_MoveBox::OnButtonClicked()
{
    AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

    //get the index of the move clicked in the move stack
    int32 ClickedMoveIdx = GameMode->ChessBoard->MoveStack.Find(Move);
    int32 CurrentMoveIdx = -1;
    TArray<ATile*> PreviousColoredTiles = {};

    //if the human player has selected a piece and then he has clicked in the storyboard, undo the first action
    AChess_HumanPlayer* HumanPlayer = Cast<AChess_HumanPlayer>(GameMode->Players[0]);
    if (IsValid(HumanPlayer) && !GameMode->ChessBoard->MoveStack.Last()->bisCheckmate)
    {   
        if (!HumanPlayer->GetFirstClick())
        {
            //set bfirstclick true again
            HumanPlayer->SetFirstClick(true);

            //restore the color of the suggested tiles and of the tile under the selected piece
            GameMode->ChessBoard->RestoreSquaresColor(HumanPlayer->GetActualMoves());
            GameMode->ChessBoard->RestoreASquareColor(HumanPlayer->GetSelectedTile());

            //set them null
            HumanPlayer->SetActualMovesEmpty();
            HumanPlayer->SetSelectedTileNull();

            //delete the move instantiated
            UMove* firstclickmove = GameMode->ChessBoard->MoveStack.Last();
            firstclickmove->ConditionalBeginDestroy();
            GameMode->ChessBoard->MoveStack.Remove(firstclickmove);
        }
    }

    //get the index of the current last move displayed on the screen and its departure and destination tiles
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
        //if another button was previously clicked, restore tiles color
        GameMode->ChessBoard->RestoreSquaresColor(PreviousColoredTiles);

        //change color to departure tile and destination tile of the move clicked
        Move->To->SetTileColor(5);
        Move->From->SetTileColor(5);

        if (ClickedMoveIdx < CurrentMoveIdx)
        {
            // ClickedMove is placed before CurrentMove in the stack, undo the moves in between
            for (int32 i = CurrentMoveIdx; i > ClickedMoveIdx; i--)
            {
                GameMode->ChessBoard->MoveStack[i]->UndoMove(GameMode);
            }
        }
        else
        {
            // ClickedMove is placed after CurrentMove in the stack, do the moves in between
            for (int32 i = CurrentMoveIdx + 1; i <= ClickedMoveIdx; i++)
            {
                GameMode->ChessBoard->MoveStack[i]->doMove(GameMode);
            }
        }

        //the last move currently visualized is Move, assign it to CurrentChessboardState
        GameMode->ChessBoard->CurrentChessboardState = Move;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("One of the moves is not in the MoveStack"));
    }
}
