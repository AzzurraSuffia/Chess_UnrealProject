// Fill out your copyright notice in the Description page of Project Settings.


#include "KingPiece.h"
#include "GameField.h"

// Sets default values
AKingPiece::AKingPiece()
{
	PrimaryActorTick.bCanEverTick = false;

}

//AL TERMINA USA validMovesChoices.Empty() PER SVUOTARE L'ARRAY
TArray<ATile*> AKingPiece::validMoves()
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
		if(ChessBoard->TileMap.Contains(ToEvaluateSquare[i]))
		{
			ATile* CurrTile = ChessBoard->TileMap[ToEvaluateSquare[i]];
		
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
