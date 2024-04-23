// Fill out your copyright notice in the Description page of Project Settings.


#include "KingPiece.h"
#include "GameField.h"

// Sets default values
AKingPiece::AKingPiece()
{
	PrimaryActorTick.bCanEverTick = false;

}

TArray<ATile*> AKingPiece::validMoves()
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
	
	//store all king's possible destinations
	TArray<FVector2D> ToEvaluateSquare = { 
		FVector2D(Xposition + 1, Yposition + 1), 
		FVector2D(Xposition + 1, Yposition),
		FVector2D(Xposition + 1, Yposition - 1),
		FVector2D(Xposition, Yposition + 1),
		FVector2D(Xposition, Yposition - 1),
		FVector2D(Xposition - 1, Yposition + 1),
		FVector2D(Xposition - 1, Yposition),
		FVector2D(Xposition - 1, Yposition - 1)};

	for (int32 i = 0; i < ToEvaluateSquare.Num(); i++) {

		//check if the searched position is still part of the chessboard
		if(ChessBoard->TileMap.Contains(ToEvaluateSquare[i]))
		{
			ATile* CurrTile = ChessBoard->TileMap[ToEvaluateSquare[i]];
				
			//if the tile is occupied by an enemy or empty, add the tile as a candidate destination tile
			if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				validMovesChoices.Add(CurrTile);
			}
		}
	}

	return validMovesChoices;
}

int32 AKingPiece::PieceWeight()
{
	return 200;
}

// Called when the game starts or when spawned
void AKingPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
// Called every frame
void AKingPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
