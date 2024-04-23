// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tile.h"
#include "ChessPiece.h"
#include "Move.generated.h"

class AChess_GameMode;

/**
 * 
 */
UCLASS()
class CHESS_UE52_API UMove : public UObject
{
	GENERATED_BODY()

public:

    UMove();
    UMove(int32 Number, ATile* A, ATile* B, AChessPiece* Piece, /*bool file, bool rank,*/ bool isCapture, AChessPiece* Captured, bool isPromotion, AChessPiece* Promoted, bool IsEnPassant, bool Check, bool Checkmate);

    //number of the move
    int32 MoveNumber;

    //departure of the piece that is moving
    ATile* From;

    //destination of the piece that is moving
    ATile* To;

    //storing the piece that is moving
    AChessPiece* PieceMoving;

    //boolean variables to implement ambigous notation if desired
    //bool bfileAmbiguity;
    //bool brankAmbiguity;

    //boolean variable to report the capture of a piece in the move
    bool bisCapture;

    //storing the piece captured
    AChessPiece* PieceCaptured;

    //boolean variable to report the promotion of a pawn in the move
    bool bisPromotion;

    //storing the piece that replaces the pawn
    AChessPiece* PiecePromoted;

    //boolean variable to report the capture of a piece in the move with en passant
    bool benPassant;

    //boolean variable to report the check of a king in the move
    bool bisCheck;

    //boolean variable to report the checkmate of a king in the move (end of game)
    bool bisCheckmate;

    //returns as an FString the Long Algebraic Notation of the move
    FString AlgebricMoveNotation();

    //update data structures and move actors in order to revert the move
    void UndoMove(AChess_GameMode* GameMode);

    //update data structures and move actors in order to confirm the move
    void doMove(AChess_GameMode* GameMode);

    //copy all attributes from OtherMove
    void CopyFrom(const UMove* OtherMove);

    //static method to copy all attributes in SourceMove in DestinationMove
    static void CopyMove(UMove* DestinationMove, const UMove* SourceMove);

    //static method to clear all the attributes of the move
    static void ClearMove(UMove* Move);
};