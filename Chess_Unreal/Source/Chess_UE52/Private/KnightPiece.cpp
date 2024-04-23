// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightPiece.h"
#include "GameField.h"

// Sets default values
AKnightPiece::AKnightPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

TArray<ATile*> AKnightPiece::validMoves()
{
	//set the tile status of tiles occupied by opponent's piece
	ETileStatus Enemy;
	if (this->PieceColor == EColor::BLACK)
	{
		Enemy = ETileStatus::WHITEPIECE;
	}
	else if (this->PieceColor == EColor::WHITE)
	{
		Enemy = ETileStatus::BLACKPIECE;
	}

	double Xposition = PlaceAt.X;
	double Yposition = PlaceAt.Y;
	TArray<ATile*> validMovesChoices;

	//check if the searched position is still part of the chessboard
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition + 2, Yposition + 1))) 
	{
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition + 2, Yposition + 1)];

		//if the tile is occupied by an enemy or empty, add the tile as a candidate destination tile
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}

	//check if the searched position is still part of the chessboard 
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition + 2, Yposition - 1)))
	{
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition + 2, Yposition - 1)];

		//if the tile is occupied by an enemy or empty, add the tile as a candidate destination tile
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}

	//check if the searched position is still part of the chessboard 
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition - 2, Yposition + 1)))
	{
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition - 2, Yposition + 1)];

		//if the tile is occupied by an enemy or empty, add the tile as a candidate destination tile
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}

	//check if the searched position is still part of the chessboard 
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition - 2, Yposition - 1)))
	{
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition - 2, Yposition - 1)];

		//if the tile is occupied by an enemy or empty, add the tile as a candidate destination tile
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}

	//check if the searched position is still part of the chessboard
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition - 1, Yposition + 2)))
	{
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition - 1, Yposition + 2)];

		//if the tile is occupied by an enemy or empty, add the tile as a candidate destination tile
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}

	//check if the searched position is still part of the chessboard
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition + 1, Yposition + 2)))
	{
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition + 1, Yposition + 2)];

		//if the tile is occupied by an enemy or empty, add the tile as a candidate destination tile
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}

	//check if the searched position is still part of the chessboard
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition - 1, Yposition - 2)))
	{
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition - 1, Yposition - 2)];

		//if the tile is occupied by an enemy or empty, add the tile as a candidate destination tile
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}

	//check if the searched position is still part of the chessboard
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition + 1, Yposition - 2)))
	{
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition + 1, Yposition - 2)];

		//if the tile is occupied by an enemy or empty, add the tile as a candidate destination tile
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}

	return validMovesChoices;
}

int32 AKnightPiece::PieceWeight()
{
	return 3;
}

// Called when the game starts or when spawned
void AKnightPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
// Called every frame
void AKnightPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
