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
    UMove(int32 Number, ATile* A, ATile* B, AChessPiece* Piece, bool file, bool rank, bool isCapture, AChessPiece* Captured, bool isPromotion, AChessPiece* Promoted, bool Check, bool Checkmate);

    int32 MoveNumber;
    ATile* From;
    ATile* To;
    AChessPiece* PieceMoving;

    /*DA CONTROLLARE*/
    bool bfileAmbiguity;
    bool brankAmbiguity;

    bool bisCapture;
    AChessPiece* PieceCaptured;

    bool bisPromotion;
    AChessPiece* PiecePromoted;

    bool bisCheck;
    bool bisCheckmate;

    FString AlgebricMoveNotation();

    void UndoMove(AChess_GameMode* GameMode);
    void doMove(AChess_GameMode* GameMode);
    void CopyFrom(const UMove* OtherMove);
    static void CopyMove(UMove* DestinationMove, const UMove* SourceMove);
    static void ClearMove(UMove* Move);
};