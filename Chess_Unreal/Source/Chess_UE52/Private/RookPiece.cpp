// Fill out your copyright notice in the Description page of Project Settings.


#include "RookPiece.h"
#include "GameField.h"

// Sets default values
ARookPiece::ARookPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

TArray<ATile*> ARookPiece::validMoves()
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

	//evaluate the tiles in the vertical axis below rook current position
	for (int32 x = PlaceAt.X+1; x < ChessBoard->Size; x++)
	{
		//check if the searched position is still part of the chessboard
		if (ChessBoard->TileMap.Contains(FVector2D(x, Yposition)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(x, PlaceAt.Y)];
						
			//if the tile is occupied by an enemy, add the tile and stop the search
			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}

			//if the tile is empty, add the tile and keep going
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}

			//if the tile is occupied by one of my pieces, stop the search
			else
			{
				break;
			}
		}
	}

	//evaluate the tiles in the vertical axis above rook current position
	for (int32 x = PlaceAt.X - 1; x >= 0; x--)
	{
		//check if the searched position is still part of the chessboard
		if (ChessBoard->TileMap.Contains(FVector2D(x, Yposition)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(x, Yposition)];

			//if the tile is occupied by an enemy, add the tile and stop the search
			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}

			//if the tile is empty, add the tile and keep going
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}

			//if the tile is occupied by one of my pieces, stop the search
			else
			{
				break;
			}
		}
	}

	//evaluate the tiles in the horizontal to the left of rook current position
	for (int32 y = PlaceAt.Y + 1; y < ChessBoard->Size; y++)
	{
		//check if the searched position is still part of the chessboard
		if (ChessBoard->TileMap.Contains(FVector2D(Xposition, y)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition, y)];
						
			//if the tile is occupied by an enemy, add the tile and stop the search
			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}

			//if the tile is empty, add the tile and keep going
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}

			//if the tile is occupied by one of my pieces, stop the search
			else
			{
				break;
			}
		}
	}

	//evaluate the tiles in the horizontal to the right of rook current position
	for (int32 y = PlaceAt.Y - 1; y >= 0; y--)
	{
		//check if the searched position is still part of the chessboard
		if (ChessBoard->TileMap.Contains(FVector2D(Xposition, y)))
		{
			//if the tile is occupied by an enemy, add the tile and stop the search
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition, y)];
			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}

			//if the tile is empty, add the tile and keep going
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}

			//if the tile is occupied by one of my pieces, stop the search
			else
			{
				break;
			}
		}	
	}
	
	return validMovesChoices;
}

int32 ARookPiece::PieceWeight()
{
	return 5;
}

// Called when the game starts or when spawned
void ARookPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
// Called every frame
void ARookPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
