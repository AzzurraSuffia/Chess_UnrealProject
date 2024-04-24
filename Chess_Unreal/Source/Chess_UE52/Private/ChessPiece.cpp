// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"
#include "Tile.h"
#include"GameField.h"
#include "Chess_GameMode.h"

// Sets default values
AChessPiece::AChessPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	PieceColor = EColor::EMPTY;
	PlaceAt = FVector2D(0, 0);

}

// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
// Called every frame
void AChessPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/

//it's valid for all pieces except pawns
bool AChessPiece::CanCaptureOpponentPiece(AChessPiece* PieceCaptured)
{
	//get all possible destination tiles
	TArray<ATile*> possibleMoves = this->validMoves();

	//if above one of those there's the piece to capture, return true
	for (ATile* validTile : possibleMoves)
	{
		if (validTile->GetGridPosition() == PieceCaptured->PlaceAt)
		{
			return true;
		}
	}

	return false;
}

int32 AChessPiece::PieceWeight()
{
	return int32();
}

TArray<ATile*> AChessPiece::validMoves()
{
	return TArray<ATile*>();
}

//it's valid for all pieces except pawns
AChessPiece* AChessPiece::doVirtualMove(AChessPiece* Piece, ATile* from, ATile* to)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	ETileStatus MyType = ETileStatus::EMPTY;
	ETileStatus OpponentType = ETileStatus::EMPTY;
	FVector2D MoveCurrPieceTo = to->GetGridPosition();
	AChessPiece* CapturedPiece = nullptr;
	TArray<AChessPiece*> OpponentPieceOnBoard = {};

	//based on the color of the piece, define its opponent and its tile status
	if (Piece->PieceColor == EColor::BLACK)
	{
		MyType = ETileStatus::BLACKPIECE; OpponentType = ETileStatus::WHITEPIECE; OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard;
	}
	else if (Piece->PieceColor == EColor::WHITE)
	{
		MyType = ETileStatus::WHITEPIECE; OpponentType = ETileStatus::BLACKPIECE; OpponentPieceOnBoard = GameMode->ChessBoard->BlackPieceOnChessBoard;
	}

	//if the destination tile passed as an argument has above an opponent's piece, it's a standard capture
	if (to->GetTileStatus() == OpponentType)
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

		//remove the piece captured from chessboard
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
	
	//set departure and destionation tiles tile status
	from->SetTileStatus(ETileStatus::EMPTY);
	to->SetTileStatus(MyType);

	//set pawn new position
	Piece->PlaceAt = MoveCurrPieceTo;

	return CapturedPiece;
}

//it's valid for all pieces except pawns
void AChessPiece::undoVirtualMove(AChessPiece* Piece, ATile* from, ATile* to, AChessPiece* CapturedPiece)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	ETileStatus MyType = ETileStatus::EMPTY;
	ETileStatus OpponentType = ETileStatus::EMPTY;

	//based on the color of the pawn, define its opponent's and its tile status
	if (Piece->PieceColor == EColor::BLACK)
	{
		MyType = ETileStatus::BLACKPIECE; OpponentType = ETileStatus::WHITEPIECE; 
	}
	else if (Piece->PieceColor == EColor::WHITE)
	{
		MyType = ETileStatus::WHITEPIECE; OpponentType = ETileStatus::BLACKPIECE;
	}

	//set departure tile tile status 
	to->SetTileStatus(MyType);

	//restore the position of the piece
	Piece->PlaceAt = to->GetGridPosition();

	//if there was a capture
	if (CapturedPiece != nullptr)
	{
		from->SetTileStatus(OpponentType);

		//restore captured piece position
		CapturedPiece->PlaceAt = from->GetGridPosition();

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
}

bool AChessPiece::IsLegal(ATile* to)
{
	ATile* from = this->ChessBoard->TileMap[this->PlaceAt];
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	bool bdoesMyMoveGenerateCheck = false;

	//update data structures to virtually execute a move
	AChessPiece* CapturedPiece = doVirtualMove(this, from, to);
	
	//based on the color of the piece, find out if the move has generated a check
	if (this->PieceColor == EColor::WHITE)
	{
		bdoesMyMoveGenerateCheck = GameMode->IsKingInCheck(0);
	}
	else if (this->PieceColor == EColor::BLACK)
	{
		bdoesMyMoveGenerateCheck= GameMode->IsKingInCheck(1);
	}

	//update data structures to virtually revert a move
	undoVirtualMove(this, to, from, CapturedPiece);

	return !bdoesMyMoveGenerateCheck;
}

void AChessPiece::SetPieceColor(EColor PColor)
{
	if (PColor == EColor::BLACK)
	{
		StaticMeshComponent->SetMaterial(0, BlackPiece);
	}
	else if (PColor == EColor::WHITE)
	{
		StaticMeshComponent->SetMaterial(0, WhitePiece);
	}
}
