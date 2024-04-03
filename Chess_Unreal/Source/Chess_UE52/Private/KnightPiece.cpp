// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightPiece.h"
#include "GameField.h"

// Sets default values
AKnightPiece::AKnightPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AddIfIsValidToJump(double InX, double InY, AGameField* ChessBoard, TArray<ATile*> validMovesChoices, ETileStatus Enemy) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("AddIfIsValidToJump"));
	ATile* CurrTile = ChessBoard->TileMap[FVector2D(InX, InY)];
	if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
	{
		validMovesChoices.Add(CurrTile);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ho aggiunto la tile"));
	}
}

//AL TERMINA USA validMovesChoices.Empty() PER SVUOTARE L'ARRAY
TArray<ATile*> AKnightPiece::validMoves()
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

	//SE UTILIZZO LE CHIAMATE A ADDIFISVALIDTOJUMP NON AGGIUNGE LE TILE E RESTITUISCE VALIDMOVESCHOICES VUOTO, NON SO PERCHE'
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition + 2, Yposition + 1))) {
		//AddIfIsValidToJump(Xposition + 2, Yposition + 1, ChessBoard, validMovesChoices, Enemy);
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition + 2, Yposition + 1)];
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition + 2, Yposition - 1)))
	{
		//AddIfIsValidToJump(PlaceAt.X + 2, PlaceAt.Y + 1, ChessBoard, validMovesChoices, Enemy);
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition + 2, Yposition - 1)];
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition - 2, Yposition + 1)))
	{
		//AddIfIsValidToJump(PlaceAt.X - 2, PlaceAt.Y + 1, ChessBoard, validMovesChoices, Enemy);
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition - 2, Yposition + 1)];
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition - 2, Yposition - 1)))
	{
		//AddIfIsValidToJump(PlaceAt.X - 2, PlaceAt.Y - 1, ChessBoard, validMovesChoices, Enemy);
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition - 2, Yposition - 1)];
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition - 1, Yposition + 2)))
	{
		//AddIfIsValidToJump(PlaceAt.X - 1, PlaceAt.Y + 2, ChessBoard, validMovesChoices, Enemy);
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition - 1, Yposition + 2)];
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition + 1, Yposition + 2)))
	{
		//AddIfIsValidToJump(PlaceAt.X + 1, PlaceAt.Y + 2, ChessBoard, validMovesChoices, Enemy);
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition + 1, Yposition + 2)];
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition - 1, Yposition - 2)))
	{
		//AddIfIsValidToJump(PlaceAt.X - 1, PlaceAt.Y - 2, ChessBoard, validMovesChoices, Enemy);
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition - 1, Yposition - 2)];
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}
	if (ChessBoard->TileMap.Contains(FVector2D(Xposition + 1, Yposition - 2)))
	{
		//AddIfIsValidToJump(PlaceAt.X + 1, PlaceAt.Y - 2, ChessBoard, validMovesChoices, Enemy);
		ATile* CurrTile = ChessBoard->TileMap[FVector2D(Xposition + 1, Yposition - 2)];
		if (CurrTile->GetTileStatus() == Enemy || CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			validMovesChoices.Add(CurrTile);
		}
	}

	return validMovesChoices;
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
