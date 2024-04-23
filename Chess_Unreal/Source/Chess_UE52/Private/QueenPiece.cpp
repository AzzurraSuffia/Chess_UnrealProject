// Fill out your copyright notice in the Description page of Project Settings.


#include "QueenPiece.h"
#include "GameField.h"

// Sets default values
AQueenPiece::AQueenPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

TArray<ATile*> AQueenPiece::validMoves()
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

	//evaluate the tiles in the vertical axis below queen current position
	for (int32 x = PlaceAt.X + 1; x < ChessBoard->Size; x++)
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

	//evaluate the tiles in the vertical axis above queen current position
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

	//evaluate the tiles in the horizontal to the left of wueen current position
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

	//evaluate the tiles in the horizontal to the right of queen current position
	for (int32 y = PlaceAt.Y - 1; y >= 0; y--)
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

	//evaluate the tiles in the lower left diagonal from queen current position
	int32 yLowerLeftDiagonal = PlaceAt.Y + 1;
	int32 xLowerLeftDiagonal = PlaceAt.X + 1;
	while (xLowerLeftDiagonal < ChessBoard->Size && yLowerLeftDiagonal < ChessBoard->Size)
	{
		//check if the searched position is still part of the chessboard
		if (ChessBoard->TileMap.Contains(FVector2D(xLowerLeftDiagonal, yLowerLeftDiagonal)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(xLowerLeftDiagonal, yLowerLeftDiagonal)];

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
			yLowerLeftDiagonal++;
			xLowerLeftDiagonal++;
		}
	}

	//evaluate the tiles in the upper left diagonal from queen current position
	int32 yUpperLeftDiagonal = PlaceAt.Y + 1;
	int32 xUpperLeftDiagonal = PlaceAt.X - 1;
	while (xUpperLeftDiagonal >= 0 && yUpperLeftDiagonal < ChessBoard->Size)
	{
		//check if the searched position is still part of the chessboard
		if (ChessBoard->TileMap.Contains(FVector2D(xUpperLeftDiagonal, yUpperLeftDiagonal)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(xUpperLeftDiagonal, yUpperLeftDiagonal)];

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
			xUpperLeftDiagonal--;
			yUpperLeftDiagonal++;
		}
	}

	// evaluate the tiles in the lower right diagonal from queen current position
	int32 yLowerRightDiagonal = PlaceAt.Y - 1;
	int32 xLowerRightDiagonal = PlaceAt.X + 1;
	while (xLowerRightDiagonal < ChessBoard->Size && yLowerRightDiagonal >= 0)
	{
		//check if the searched position is still part of the chessboard
		if (ChessBoard->TileMap.Contains(FVector2D(xLowerRightDiagonal, yLowerRightDiagonal)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(xLowerRightDiagonal, yLowerRightDiagonal)];

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
			xLowerRightDiagonal++;
			yLowerRightDiagonal--;
		}
	}

	//evaluate the tiles in the upper right diagonal from queen current position
	int32 yUpperRightDiagonal = PlaceAt.Y - 1;
	int32 xUpperRightDiagonal = PlaceAt.X - 1;
	while (xUpperRightDiagonal >= 0 && yUpperRightDiagonal >= 0)
	{
		//check if the searched position is still part of the chessboard
		if (ChessBoard->TileMap.Contains(FVector2D(xUpperRightDiagonal, yUpperRightDiagonal)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(xUpperRightDiagonal, yUpperRightDiagonal)];

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
			xUpperRightDiagonal--;
			yUpperRightDiagonal--;
		}
	}


	return validMovesChoices;
}

int32 AQueenPiece::PieceWeight()
{
	return 9;
}

// Called when the game starts or when spawned
void AQueenPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
// Called every frame
void AQueenPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/