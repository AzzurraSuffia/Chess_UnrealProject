// Fill out your copyright notice in the Description page of Project Settings.


#include "Move.h"
#include "Chess_GameMode.h"

TArray<FString> NumberToLetter = { FString("a"), FString("b"), FString("c"), FString("d"), FString("e"), FString("f"), FString("g"), FString("h") };

EPieceNotation PieceToNotation(AChessPiece* Piece)
{
	if (APawnPiece* Pawn = Cast<APawnPiece>(Piece))
	{
		return EPieceNotation::P;
	}
	else if (ARookPiece* Rook = Cast<ARookPiece>(Piece))
	{
		return EPieceNotation::R;
	}
	else if (AKnightPiece* Knight = Cast<AKnightPiece>(Piece))
	{
		return EPieceNotation::N;
	}
	else if (ABishopPiece* Bishop = Cast<ABishopPiece>(Piece))
	{
		return EPieceNotation::B;
	}
	else if (AQueenPiece* Queen = Cast<AQueenPiece>(Piece))
	{
		return EPieceNotation::Q;
	}
	else if (AKingPiece* King = Cast<AKingPiece>(Piece))
	{
		return EPieceNotation::K;
	}
	else
	{
		return EPieceNotation::EMPTY;
	}
}

UMove::UMove()
{
	MoveNumber = 0;
	From = nullptr;
	To = nullptr;
	PieceMoving = nullptr;
	bfileAmbiguity = false;
	brankAmbiguity = false;
	bisCapture = false;
	PieceCaptured = nullptr;
	bisPromotion = false;
	PiecePromoted = nullptr;
	OriginalPawn = nullptr;
	bisCheck = false;
	bisCheckmate = false;
}

UMove::UMove(int32 Number, ATile* A, ATile* B, AChessPiece* Piece, bool file, bool rank, bool isCapture, AChessPiece* Captured, bool isPromotion, AChessPiece* Promoted, AChessPiece* Pawn, bool Check, bool Checkmate)
{
	MoveNumber = Number;
	From = A;
	To = B;
	PieceMoving = Piece;
	bfileAmbiguity = file;
	brankAmbiguity = rank;
	bisCapture = isCapture;
	PieceCaptured = Captured;
	bisPromotion = isPromotion;
	PiecePromoted = Promoted; 
	OriginalPawn = Pawn;
	bisCheck = Check;
	bisCheckmate = Checkmate;
}

FString UMove::AlgebricMoveNotation()
{
	FString MoveNotation = FString();

	if (PieceMoving != nullptr && From != nullptr && To != nullptr && MoveNumber != 0)
	{

		EPieceNotation Letter = PieceToNotation(PieceMoving);
		switch (Letter)
		{
		case EPieceNotation::R: MoveNotation.Append("R"); break;
		case EPieceNotation::B: MoveNotation.Append("B"); break;
		case EPieceNotation::N: MoveNotation.Append("N"); break;
		case EPieceNotation::Q: MoveNotation.Append("Q"); break;
		case EPieceNotation::K: MoveNotation.Append("K"); break;
		default:
		//per il pedone non va indicato la lettera rappresentante il pezzo
			MoveNotation.Append("");
		}
		
		if (bfileAmbiguity)
		{
			int32 Yfrom = From->GetGridPosition().Y;
			MoveNotation.Append(NumberToLetter[Yfrom]);
		} 
		else if (brankAmbiguity)
		{
			int32 Xfrom = To->GetGridPosition().X;
			MoveNotation.Append(FString::FromInt(Xfrom + 1));
		}

		if (bisCapture)
		{
			if (Letter == EPieceNotation::P && !bfileAmbiguity)
			{
				int32 Ypawn = From->GetGridPosition().Y;
				MoveNotation.Append(NumberToLetter[Ypawn]);
			}

			MoveNotation.Append("x");
		}

		int32 Xposition = To->GetGridPosition().X; 
		int32 Yposition = To->GetGridPosition().Y;
		MoveNotation.Append(NumberToLetter[Yposition]);
		MoveNotation.Append(FString::FromInt(Xposition + 1));

		if (bisPromotion)
		{
			switch (PieceToNotation(PiecePromoted))
			{
			case EPieceNotation::R: MoveNotation.Append("R"); break;
			case EPieceNotation::B: MoveNotation.Append("B"); break;
			case EPieceNotation::N: MoveNotation.Append("N"); break;
			default:
				MoveNotation.Append("Q");
			}
		}

		if (bisCheck)
		{
			MoveNotation.Append("+");
		} 
		else if (bisCheckmate)
		{
			MoveNotation.Append("#");
		}
	}

	return MoveNotation;
}

void UMove::UndoLastMove()
{
	/*
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	ETileStatus MyType = ETileStatus::EMPTY;
	ETileStatus OpponentType = ETileStatus::EMPTY;

	if (PieceMoving->PieceColor == EColor::BLACK)
	{
		MyType = ETileStatus::BLACKPIECE; OpponentType = ETileStatus::WHITEPIECE;
	}
	else if (PieceMoving->PieceColor == EColor::WHITE)
	{
		MyType = ETileStatus::WHITEPIECE; OpponentType = ETileStatus::BLACKPIECE;
	}

	To->SetTileStatus(MyType);
	PieceMoving->PlaceAt = To->GetGridPosition();

	if (PieceCaptured != nullptr)
	{
		//DEVO RIPRISTINARE IL PEZZO CATTURATO
		From->SetTileStatus(OpponentType);
		PieceCaptured->PlaceAt = From->GetGridPosition();
		if (PieceMoving->PieceColor == EColor::BLACK)
		{
			GameMode->ChessBoard->WhitePieceOnChessBoard.Add(PieceCaptured);
		}
		else if (PieceMoving->PieceColor == EColor::WHITE)
		{
			GameMode->ChessBoard->BlackPieceOnChessBoard.Add(PieceCaptured);
		}
	}
	else
	{
		From->SetTileStatus(ETileStatus::EMPTY);
	}
	*/

	PieceMoving->undoVirtualMove(PieceMoving, To, From, PieceCaptured);

}