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
	/*POSSIBLE MOVES NON AVRA' MAI IL PROPRIO RE NELLE MOSSE POSSIBILI*/
	/*QUESTO VA BENE PER I PAWN PERCHE' PRELEVA SOLO LE CELLE DIAGONALI*/
	bool mantainValue = this->bfirstMove;
	TArray<ATile*> possibleMoves = this->validMoves();
	for (ATile* validTile : possibleMoves)
	{
		if (validTile->GetGridPosition().X != this->PlaceAt.X && validTile->GetGridPosition() == PieceCaptured->PlaceAt)
		{
			return true;
		}
	}
	this->bfirstMove = mantainValue;
	return false;
}

//AL TERMINA USA validMovesChoices.Empty() PER SVUOTARE L'ARRAY
TArray<ATile*> APawnPiece::validMoves()
{
	//poco efficiente fare questo calcolo ogni volta 
	//o lo fai una volta all'inizio o lo metti come attributo
	ETileStatus Enemy;
	double OneXStep = 0;
	double TwoXStep = 0;
	double Xposition = PlaceAt.X;
	double Yposition = PlaceAt.Y;
	TArray<ATile*> validMovesChoices;

	if (this->PieceColor == EColor::BLACK)
	{
		Enemy = ETileStatus::WHITEPIECE;
		OneXStep = PlaceAt.X - 1;
		TwoXStep = PlaceAt.X - 2;
	}
	else if (this->PieceColor == EColor::WHITE)
	{
		Enemy = ETileStatus::BLACKPIECE;
		OneXStep = PlaceAt.X + 1;
		TwoXStep = PlaceAt.X + 2;
	}

	//controlli sul fatto che Xposition-1 ad esempio potrebbe essere fuori dal campo 
	if (ChessBoard->TileMap.Contains(FVector2D(OneXStep, Yposition - 1)))
	{
		ATile* DiagonalLeftSquare = ChessBoard->TileMap[FVector2D(OneXStep, Yposition - 1)];
		if (DiagonalLeftSquare->GetTileStatus() == Enemy)
		{
			validMovesChoices.Add(DiagonalLeftSquare);
		}
	}

	if (ChessBoard->TileMap.Contains(FVector2D(OneXStep, Yposition + 1)))
	{
		ATile* DiagonalRightSquare = ChessBoard->TileMap[FVector2D(OneXStep, Yposition + 1)];
		if (DiagonalRightSquare->GetTileStatus() == Enemy)
		{
			validMovesChoices.Add(DiagonalRightSquare);
		}
	}

	if (ChessBoard->TileMap.Contains(FVector2D(OneXStep, Yposition)))
	{
		ATile* FirstSquare = ChessBoard->TileMap[FVector2D(OneXStep, Yposition)];
		if (FirstSquare->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(FirstSquare);

			if (bfirstMove)
			{
			bfirstMove = false;
			//alla sua prima mossa un pedone ha sempre almeno due caselle davanti
			//if (ChessBoard->TileMap.Contains(FVector2D(TwoXStep, Yposition)))
			// {
				ATile* SecondSquare = ChessBoard->TileMap[FVector2D(TwoXStep, Yposition)];
				if (SecondSquare->GetTileStatus() == ETileStatus::EMPTY) 
				{
					validMovesChoices.Add(SecondSquare);
				}
			//}
			}
		}
	}

	

	return validMovesChoices;
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
