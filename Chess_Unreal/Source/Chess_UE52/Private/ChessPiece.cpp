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

bool AChessPiece::CanCaptureOpponentPiece(AChessPiece* PieceCaptured)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("CanCaptureOpponentPiece"));
	/*POSSIBLE MOVES NON AVRA' MAI IL PROPRIO RE NELLE MOSSE POSSIBILI*/
	/*QUESTO VA BENE PER TUTTI TRANNE CHE PER I PAWN CHE POSSONO CATTURARE SOLO IN DIAGONALE*/
	TArray<ATile*> possibleMoves = this->validMoves();
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

/*DEVO SIMULARE LA MOSSA AGGIORNANDO LE STRUTTURE DATI MA SENZA MUOVERE L'ATTORE*/
AChessPiece* AChessPiece::doVirtualMove(AChessPiece* Piece, ATile* from, ATile* to)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	ETileStatus MyType = ETileStatus::EMPTY;
	ETileStatus OpponentType = ETileStatus::EMPTY;
	FVector2D MoveCurrPieceTo = to->GetGridPosition();
	AChessPiece* CapturedPiece = nullptr;
	TArray<AChessPiece*> OpponentPieceOnBoard = {};

	if (Piece->PieceColor == EColor::BLACK)
	{
		MyType = ETileStatus::BLACKPIECE; OpponentType = ETileStatus::WHITEPIECE; OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard;
	}
	else if (Piece->PieceColor == EColor::WHITE)
	{
		MyType = ETileStatus::WHITEPIECE; OpponentType = ETileStatus::BLACKPIECE; OpponentPieceOnBoard = GameMode->ChessBoard->BlackPieceOnChessBoard;
	}

	if (to->GetTileStatus() == OpponentType)
	{
		/*CATTURA DI UN PEZZO*/
		int32 Size = OpponentPieceOnBoard.Num();
		for (int32 i = 0; i < Size; i++)
		{
			if (OpponentPieceOnBoard[i]->PlaceAt == MoveCurrPieceTo)
			{
				CapturedPiece = OpponentPieceOnBoard[i];
				break;
			}
		}
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
	
	from->SetTileStatus(ETileStatus::EMPTY);
	to->SetTileStatus(MyType);
	Piece->PlaceAt = MoveCurrPieceTo;

	return CapturedPiece;
}

/*DEVO RIPRISTINARE LA SITUAZIONE ORIGINALE RIPORTANDO ALLO STATO INIZIALE LE STRUTTURE DATI*/
void AChessPiece::undoVirtualMove(AChessPiece* Piece, ATile* from, ATile* to, AChessPiece* CapturedPiece)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	ETileStatus MyType = ETileStatus::EMPTY;
	ETileStatus OpponentType = ETileStatus::EMPTY;

	if (Piece->PieceColor == EColor::BLACK)
	{
		MyType = ETileStatus::BLACKPIECE; OpponentType = ETileStatus::WHITEPIECE; 
	}
	else if (Piece->PieceColor == EColor::WHITE)
	{
		MyType = ETileStatus::WHITEPIECE; OpponentType = ETileStatus::BLACKPIECE;
	}

	to->SetTileStatus(MyType);
	Piece->PlaceAt = to->GetGridPosition();

	if (CapturedPiece != nullptr)
	{
		/*DEVO RIPRISTINARE IL PEZZO CATTURATO*/
		from->SetTileStatus(OpponentType);
		CapturedPiece->PlaceAt = from->GetGridPosition();
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("IS LEGAL"));
	ATile* from = this->ChessBoard->TileMap[this->PlaceAt];
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	bool bdoesMyMoveGenerateCheck = false;

	/*DEVO SIMULARE LA MOSSA AGGIORNANDO LE STRUTTURE DATI MA SENZA MUOVERE L'ATTORE*/
	AChessPiece* CapturedPiece = doVirtualMove(this, from, to);
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("IsLegal"));
	if (this->PieceColor == EColor::WHITE)
	{
		//devo controllare se la mia mossa mette in scacco il mio re
		bdoesMyMoveGenerateCheck = GameMode->IsKingInCheck(0);
	}
	else if (this->PieceColor == EColor::BLACK)
	{
		//devo controllare se la mia mossa mette in scacco il mio re
		bdoesMyMoveGenerateCheck= GameMode->IsKingInCheck(1);
	}

	/*DEVO RIPRISTINARE LA SITUAZIONE ORIGINALE RIPORTANDO ALLO STATO INIZIALE LE STRUTTURE DATI*/
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
