// Fill out your copyright notice in the Description page of Project Settings.


#include "RookPiece.h"
#include "GameField.h"

// Sets default values
ARookPiece::ARookPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

//AL TERMINA USA validMovesChoices.Empty() PER SVUOTARE L'ARRAY
TArray<ATile*> ARookPiece::validMoves()
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

	for (int32 x = PlaceAt.X+1; x < ChessBoard->Size; x++)
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
	
	return validMovesChoices;
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
