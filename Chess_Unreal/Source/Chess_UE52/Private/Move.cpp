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
	benPassant = false;
	bisCheck = false;
	bisCheckmate = false;
}

UMove::UMove(int32 Number, ATile* A, ATile* B, AChessPiece* Piece, bool file, bool rank, bool isCapture, AChessPiece* Captured, bool isPromotion, AChessPiece* Promoted, bool IsEnPassant, bool Check, bool Checkmate)
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
	benPassant = IsEnPassant;
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

		if (bisCapture || benPassant)
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

void UMove::UndoMove(AChess_GameMode* GameMode)
{
	/*
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

	/*funzione che aggiorna tutte le strutture dati, devo solo spostare gli attori */
	/*grazie all'override aggiorna le strutture dati per l'en passant da sè */
	PieceMoving->undoVirtualMove(PieceMoving, To, From, PieceCaptured);

	APawnPiece* Pawn = Cast<APawnPiece>(PieceMoving);
	if (IsValid(Pawn))
	{
		int32 PawnStartPosition = -1;
		if (PieceMoving->PieceColor == EColor::WHITE)
		{
			PawnStartPosition = 1;
		}
		else
		{
			PawnStartPosition = (GameMode->ChessBoard->Size - 2);
		}

		if (From->GetGridPosition().X == PawnStartPosition)
		{
			Pawn->bfirstMove = true;
		}
	}

	FVector Location = GameMode->ChessBoard->GetRelativeLocationByXYPosition(PieceMoving->PlaceAt.X, PieceMoving->PlaceAt.Y);
	FVector NewLocation = Location + FVector(6, 6, 20);
	PieceMoving->SetActorLocation(NewLocation);

	if (bisCapture)
	{
		Location = GameMode->ChessBoard->GetRelativeLocationByXYPosition(PieceCaptured->PlaceAt.X, PieceCaptured->PlaceAt.Y);
		NewLocation = Location + FVector(6, 6, 20);
		PieceCaptured->SetActorLocation(NewLocation);
		PieceCaptured->SetActorHiddenInGame(false);
	} 
	else if (benPassant)
	{
		Location = GameMode->ChessBoard->GetRelativeLocationByXYPosition(PieceCaptured->PlaceAt.X, PieceCaptured->PlaceAt.Y);
		NewLocation = Location + FVector(6, 6, 20);
		PieceCaptured->SetActorLocation(NewLocation);
		PieceCaptured->SetActorHiddenInGame(false);
	}

	if (bisPromotion)
	{
		if (PiecePromoted->PieceColor == EColor::WHITE)
		{
			GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(PiecePromoted);
			GameMode->ChessBoard->WhitePieceOnChessBoard.Add(PieceMoving);
		}
		else
		{
			GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(PiecePromoted);
			GameMode->ChessBoard->BlackPieceOnChessBoard.Add(PieceMoving);
		}

		PiecePromoted->SetActorHiddenInGame(true);
		GameMode->ChessBoard->MoveOutOfChessBoard(PiecePromoted);
		PieceMoving->SetActorHiddenInGame(false);
	}
}

void UMove::doMove(AChess_GameMode* GameMode)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("doMove"));
	/*
	AChessPiece* AChessPiece::doVirtualMove(AChessPiece * Piece, ATile * from, ATile * to)
	{
		AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

		ETileStatus MyType = ETileStatus::EMPTY;
		ETileStatus OpponentType = ETileStatus::EMPTY;
		FVector2D MoveCurrPieceTo = to->GetGridPosition();
		AChessPiece* CapturedPiece = nullptr;
		TArray<AChessPiece*> OpponentPieceOnBoard = {};

		if (Piece->PieceColor == EColor::BLACK)
		{
			MyType = ETileStatus::BLACKPIECE; OpponentType = ETileStatus::WHITEPIECE; OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard;
		}
		else if (Piece->PieceColor == EColor::WHITE)
		{
			MyType = ETileStatus::WHITEPIECE; OpponentType = ETileStatus::BLACKPIECE; OpponentPieceOnBoard = GameMode->ChessBoard->BlackPieceOnChessBoard;
		}

		if (to->GetTileStatus() == OpponentType)
		{
			//CATTURA DI UN PEZZO
			int32 Size = OpponentPieceOnBoard.Num();
			for (int32 i = 0; i < Size; i++)
			{
				if (OpponentPieceOnBoard[i]->PlaceAt == MoveCurrPieceTo)
				{
					CapturedPiece = OpponentPieceOnBoard[i];
					break;
				}
			}
			if (CapturedPiece != nullptr)
			{
				if (Piece->PieceColor == EColor::BLACK)
				{
					GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(CapturedPiece);
				}
				else if (Piece->PieceColor == EColor::WHITE)
				{
					GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(CapturedPiece);
				}
			}
		}

		from->SetTileStatus(ETileStatus::EMPTY);
		to->SetTileStatus(MyType);
		Piece->PlaceAt = MoveCurrPieceTo;

		return CapturedPiece;
	}
	*/
	/*grazie all'override aggiorna le strutture dati per l'en passant da sè */
	AChessPiece* Captured = PieceMoving->doVirtualMove(PieceMoving, From, To);

	APawnPiece* Pawn = Cast<APawnPiece>(PieceMoving);
	if (IsValid(Pawn))
	{
		int32 PawnStartPosition = -1;
		if (PieceMoving->PieceColor == EColor::WHITE)
		{
			PawnStartPosition = 1;
		}
		else
		{
			PawnStartPosition = (GameMode->ChessBoard->Size - 2);
		}

		if (From->GetGridPosition().X == PawnStartPosition)
		{
			Pawn->bfirstMove = false;
		}
	}

	FVector Location = GameMode->ChessBoard->GetRelativeLocationByXYPosition(PieceMoving->PlaceAt.X, PieceMoving->PlaceAt.Y);
	FVector NewLocation = Location + FVector(6, 6, 20);
	PieceMoving->SetActorLocation(NewLocation);

	if (bisCapture && Captured == PieceCaptured)
	{
		GameMode->ChessBoard->MoveOutOfChessBoard(PieceCaptured);
		//PieceCaptured->SetActorHiddenInGame(true);
	}
	else if (benPassant && Captured == PieceCaptured)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("IN IF DI DO MOVE"));
		GameMode->ChessBoard->MoveOutOfChessBoard(PieceCaptured);
		//PieceCaptured->SetActorHiddenInGame(true);
	}

	if (bisPromotion)
	{
		if (PiecePromoted->PieceColor == EColor::WHITE)
		{
			GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(PieceMoving);
			GameMode->ChessBoard->WhitePieceOnChessBoard.Add(PiecePromoted);
		}
		else
		{
			GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(PieceMoving);
			GameMode->ChessBoard->BlackPieceOnChessBoard.Add(PiecePromoted);
		}

		PiecePromoted->PlaceAt = PieceMoving->PlaceAt;
		GameMode->ChessBoard->MoveOutOfChessBoard(PieceMoving);
		//OriginalPawn->SetActorHiddenInGame(true);
		Location = GameMode->ChessBoard->GetRelativeLocationByXYPosition(PiecePromoted->PlaceAt.X, PiecePromoted->PlaceAt.Y);
		NewLocation = Location + FVector(6, 6, 20);
		PiecePromoted->SetActorLocation(NewLocation);
		PiecePromoted->SetActorHiddenInGame(false);
	}
}

void UMove::CopyFrom(const UMove* OtherMove)
{
	//Copy attributes one by one
	MoveNumber = OtherMove->MoveNumber;
	From = OtherMove->From;
	To = OtherMove->To;
	PieceMoving = OtherMove->PieceMoving;
	bfileAmbiguity = OtherMove->bfileAmbiguity;
	brankAmbiguity = OtherMove->brankAmbiguity;
	bisCapture = OtherMove->bisCapture;
	PieceCaptured = OtherMove->PieceCaptured;
	bisPromotion = OtherMove->bisPromotion;
	PiecePromoted = OtherMove->PiecePromoted;
	benPassant = OtherMove->benPassant;
	bisCheck = OtherMove->bisCheck;
	bisCheckmate = OtherMove->bisCheckmate;
}

void UMove::CopyMove(UMove* DestinationMove, const UMove* SourceMove)
{
	if (DestinationMove && SourceMove)
	{
		DestinationMove->CopyFrom(SourceMove);
	}
}

void UMove::ClearMove(UMove* Move)
{
	//clear all the move fields
	Move->MoveNumber = 0;
	Move->PieceMoving = nullptr;
	Move->To = nullptr;
	Move->From = nullptr;
	Move->bfileAmbiguity = false;
	Move->brankAmbiguity = false;
	Move->bisCapture = false;
	Move->PieceCaptured = nullptr;
	Move->bisPromotion = false;
	Move->PiecePromoted = nullptr;
	Move->benPassant = false;
	Move->bisCheck = false;
	Move->bisCheckmate = false;
}
