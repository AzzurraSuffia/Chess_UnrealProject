// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPiece.h"
#include "GameField.h"
#include "Chess_GameMode.h"

// Sets default values
APawnPiece::APawnPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bfirstMove = true;
}

bool APawnPiece::CanCaptureOpponentPiece(AChessPiece* PieceCaptured)
{
	//do not alter the flag for pawn first move, store it and reassign it later
	bool mantainValue = this->bfirstMove;
	TArray<ATile*> possibleMoves = this->validMoves();
	for (ATile* validTile : possibleMoves)
	{	
		//keep only diagonal squares and check if one of them is occupied by the piece passed as argument
		if (validTile->GetGridPosition().X != this->PlaceAt.X && validTile->GetGridPosition() == PieceCaptured->PlaceAt)
		{
			return true;
		}
	}
	this->bfirstMove = mantainValue;
	return false;
}

TArray<ATile*> APawnPiece::validMoves()
{
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());

	ETileStatus Enemy;
	double OneXStep = 0;
	double TwoXStep = 0;
	double Xposition = PlaceAt.X;
	double Yposition = PlaceAt.Y;
	int32 OneStep = 0;
	TArray<ATile*> validMovesChoices;

	//based on the color of the pawn, define its opponent and its direction
	if (this->PieceColor == EColor::BLACK)
	{
		Enemy = ETileStatus::WHITEPIECE;
		OneXStep = PlaceAt.X - 1;
		TwoXStep = PlaceAt.X - 2;
		OneStep = -1;
	}
	else if (this->PieceColor == EColor::WHITE)
	{
		Enemy = ETileStatus::BLACKPIECE;
		OneXStep = PlaceAt.X + 1;
		TwoXStep = PlaceAt.X + 2;
		OneStep = 1;
	}

	// check if the searched position is still part of the chessboard
	if (ChessBoard->TileMap.Contains(FVector2D(OneXStep, Yposition - 1)))
	{
		ATile* DiagonalLeftSquare = ChessBoard->TileMap[FVector2D(OneXStep, Yposition - 1)];

		//if the diagonal tile is occupied by an enemy, add the tile
		if (DiagonalLeftSquare->GetTileStatus() == Enemy)
		{
			validMovesChoices.Add(DiagonalLeftSquare);
		}
	}

	// check if the searched position is still part of the chessboard
	if (ChessBoard->TileMap.Contains(FVector2D(OneXStep, Yposition + 1)))
	{
		ATile* DiagonalRightSquare = ChessBoard->TileMap[FVector2D(OneXStep, Yposition + 1)];

		//if the diagonal tile is occupied by an enemy, add the tile
		if (DiagonalRightSquare->GetTileStatus() == Enemy)
		{
			validMovesChoices.Add(DiagonalRightSquare);
		}
	}

	// check if the searched position is still part of the chessboard
	if (ChessBoard->TileMap.Contains(FVector2D(OneXStep, Yposition)))
	{
		ATile* FirstSquare = ChessBoard->TileMap[FVector2D(OneXStep, Yposition)];

		//if the first square front is empty, add it
		if (FirstSquare->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(FirstSquare);

			//if it's pawn first move
			if (bfirstMove)
			{
				bfirstMove = false;

				// check is not needed: on their first move pawns have always two squares in front of them
				ATile* SecondSquare = ChessBoard->TileMap[FVector2D(TwoXStep, Yposition)];

				//if the second square front is empty, add it
				if (SecondSquare->GetTileStatus() == ETileStatus::EMPTY) 
				{
					validMovesChoices.Add(SecondSquare);
				}
			}
		}
	}

	return validMovesChoices;
}

int32 APawnPiece::PieceWeight()
{
	return 1;
}

AChessPiece* APawnPiece::doVirtualMove(AChessPiece* Piece, ATile* from, ATile* to)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	ETileStatus MyType = ETileStatus::EMPTY;
	ETileStatus OpponentType = ETileStatus::EMPTY;
	FVector2D MoveCurrPieceTo = to->GetGridPosition();
	AChessPiece* CapturedPiece = nullptr;
	TArray<AChessPiece*> OpponentPieceOnBoard = {};
	int32 OneStep = 0;

	//based on the color of the pawn, define its opponent, its direction and its tile status
	if (Piece->PieceColor == EColor::BLACK)
	{
		MyType = ETileStatus::BLACKPIECE; OpponentType = ETileStatus::WHITEPIECE; OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard; OneStep = -1;
	}
	else if (Piece->PieceColor == EColor::WHITE)
	{
		MyType = ETileStatus::WHITEPIECE; OpponentType = ETileStatus::BLACKPIECE; OpponentPieceOnBoard = GameMode->ChessBoard->BlackPieceOnChessBoard; OneStep = 1;
	}

	//if a diagonal empty tile is passed, the move has to be managed as an en passant
	if (to->GetTileStatus() == ETileStatus::EMPTY && to->GetGridPosition().Y != from->GetGridPosition().Y)
	{
		//get opponent's pawn position
		FVector2D OpponentPawnPosition = FVector2D(to->GetGridPosition().X - OneStep, to->GetGridPosition().Y);

		//get opponent's pawn reference
		int32 Size = OpponentPieceOnBoard.Num();
		for (int32 i = 0; i < Size; i++)
		{
			if (OpponentPieceOnBoard[i]->PlaceAt == OpponentPawnPosition)
			{
				CapturedPiece = OpponentPieceOnBoard[i];
				break;
			}
		}

		//set tile occupied by the opponent's pawn as empty
		GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(ETileStatus::EMPTY);
	}

	//if a diagonal occupied tile is passed, the move has to be managed as a standard capture
	else if (to->GetTileStatus() == OpponentType)
	{
		//get opponent's piece reference
		int32 Size = OpponentPieceOnBoard.Num();
		for (int32 i = 0; i < Size; i++)
		{
			if (OpponentPieceOnBoard[i]->PlaceAt == MoveCurrPieceTo)
			{
				CapturedPiece = OpponentPieceOnBoard[i];
				break;
			}
		}
	}

	//if there was a capture, remove the piece captured from chessboard
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

	//set departure and destionation tiles tile status
	from->SetTileStatus(ETileStatus::EMPTY);
	to->SetTileStatus(MyType);

	//set pawn new position
	Piece->PlaceAt = MoveCurrPieceTo;

	return CapturedPiece;
}

void APawnPiece::undoVirtualMove(AChessPiece* Piece, ATile* from, ATile* to, AChessPiece* CapturedPiece)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	
	ETileStatus MyType = ETileStatus::EMPTY;
	ETileStatus OpponentType = ETileStatus::EMPTY;
	int32 OneStep = 0;

	//based on the color of the pawn, define its opponent, its direction and its tile status
	if (Piece->PieceColor == EColor::BLACK)
	{
		MyType = ETileStatus::BLACKPIECE; OpponentType = ETileStatus::WHITEPIECE; OneStep = -1;
	}
	else if (Piece->PieceColor == EColor::WHITE)
	{
		MyType = ETileStatus::WHITEPIECE; OpponentType = ETileStatus::BLACKPIECE; OneStep = 1;
	}

	//if there was a capture
	if (CapturedPiece != nullptr)
	{
		//if captured piece position and pawn position are different, it's an en passant
		if (CapturedPiece->PlaceAt.X != Piece->PlaceAt.X)
		{
			from->SetTileStatus(ETileStatus::EMPTY);

			//get opponent's pawn position and restore it
			FVector2D OpponentPawnPosition = FVector2D(from->GetGridPosition().X - OneStep, from->GetGridPosition().Y);
			GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(OpponentType);
			CapturedPiece->PlaceAt = OpponentPawnPosition;
		}
		//if they are the same, it's a standard capture
		else
		{
			from->SetTileStatus(OpponentType);

			//restore captured piece position
			CapturedPiece->PlaceAt = from->GetGridPosition();
		}

		//add the piece captured to the chessboard
		if (Piece->PieceColor == EColor::BLACK)
		{
			GameMode->ChessBoard->WhitePieceOnChessBoard.Add(CapturedPiece);
		}
		else if (Piece->PieceColor == EColor::WHITE)
		{
			GameMode->ChessBoard->BlackPieceOnChessBoard.Add(CapturedPiece);
		}
	}
	else
	{
		from->SetTileStatus(ETileStatus::EMPTY);
	}	

	//set departure tile tile status
	to->SetTileStatus(MyType);

	//set pawn previous position
	Piece->PlaceAt = to->GetGridPosition();
}

// Called when the game starts or when spawned
void APawnPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
// Called every frame
void APawnPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
