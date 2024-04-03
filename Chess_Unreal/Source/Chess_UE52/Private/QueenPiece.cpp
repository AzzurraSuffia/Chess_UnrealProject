// Fill out your copyright notice in the Description page of Project Settings.


#include "QueenPiece.h"
#include "GameField.h"

// Sets default values
AQueenPiece::AQueenPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}


//AL TERMINA USA validMovesChoices.Empty() PER SVUOTARE L'ARRAY
TArray<ATile*> AQueenPiece::validMoves()
{
	//poco efficiente fare questo calcolo ogni volta 
	//o lo fai una volta all'inizio o lo metti come attributo
	ETileStatus Enemy;
	if (this->PieceColor == EColor::BLACK)
	{
		Enemy = ETileStatus::WHITEPIECE;
	}
	else if (this->PieceColor == EColor::WHITE)
	{
		Enemy = ETileStatus::BLACKPIECE;
	}

	//Aggiungere controlli sul fatto che Xposition-1 ad esempio potrebbe essere fuori dal campo (li fa IsValid)
	double Xposition = PlaceAt.X;
	double Yposition = PlaceAt.Y;
	TArray<ATile*> validMovesChoices;

	for (int32 x = PlaceAt.X + 1; x < ChessBoard->Size; x++)
	{
		if (ChessBoard->TileMap.Contains(FVector2D(x, Yposition)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(x, PlaceAt.Y)];

			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}
			else
			{
				break;
			}
		}
	}

	for (int32 x = PlaceAt.X - 1; x >= 0; x--)
	{
		if (ChessBoard->TileMap.Contains(FVector2D(x, Yposition)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(x, Yposition)];

			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}
			else
			{
				break;
			}
		}
	}

	for (int32 y = PlaceAt.Y + 1; y < ChessBoard->Size; y++)
	{
		if (ChessBoard->TileMap.Contains(FVector2D(Xposition, y)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition, y)];
			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}
			else
			{
				break;
			}
		}
	}

	for (int32 y = PlaceAt.Y - 1; y >= 0; y--)
	{
		if (ChessBoard->TileMap.Contains(FVector2D(Xposition, y)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition, y)];
			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}
			else
			{
				break;
			}
		}
	}

	int32 yLowerLeftDiagonal = PlaceAt.Y + 1;
	int32 xLowerLeftDiagonal = PlaceAt.X + 1;
	while (xLowerLeftDiagonal < ChessBoard->Size && yLowerLeftDiagonal < ChessBoard->Size)
	{
		if (ChessBoard->TileMap.Contains(FVector2D(xLowerLeftDiagonal, yLowerLeftDiagonal)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(xLowerLeftDiagonal, yLowerLeftDiagonal)];
			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}
			else
			{
				break;
			}
			yLowerLeftDiagonal++;
			xLowerLeftDiagonal++;
		}
	}

	int32 yUpperLeftDiagonal = PlaceAt.Y + 1;
	int32 xUpperLeftDiagonal = PlaceAt.X - 1;
	while (xUpperLeftDiagonal >= 0 && yUpperLeftDiagonal < ChessBoard->Size)
	{
		if (ChessBoard->TileMap.Contains(FVector2D(xUpperLeftDiagonal, yUpperLeftDiagonal)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(xUpperLeftDiagonal, yUpperLeftDiagonal)];
			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}
			else
			{
				break;
			}
			xUpperLeftDiagonal--;
			yUpperLeftDiagonal++;
		}
	}

	int32 yLowerRightDiagonal = PlaceAt.Y - 1;
	int32 xLowerRightDiagonal = PlaceAt.X + 1;
	while (xLowerRightDiagonal < ChessBoard->Size && yLowerRightDiagonal >= 0)
	{
		if (ChessBoard->TileMap.Contains(FVector2D(xLowerRightDiagonal, yLowerRightDiagonal)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(xLowerRightDiagonal, yLowerRightDiagonal)];
			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}
			else
			{
				break;
			}
			xLowerRightDiagonal++;
			yLowerRightDiagonal--;
		}
	}

	int32 yUpperRightDiagonal = PlaceAt.Y - 1;
	int32 xUpperRightDiagonal = PlaceAt.X - 1;
	while (xUpperRightDiagonal >= 0 && yUpperRightDiagonal >= 0)
	{
		if (ChessBoard->TileMap.Contains(FVector2D(xUpperRightDiagonal, yUpperRightDiagonal)))
		{
			ATile* CurrTile = ChessBoard->TileMap[FVector2D(xUpperRightDiagonal, yUpperRightDiagonal)];
			if (CurrTile->GetTileStatus() == Enemy)
			{
				validMovesChoices.Add(CurrTile);
				break;
			}
			else if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}
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