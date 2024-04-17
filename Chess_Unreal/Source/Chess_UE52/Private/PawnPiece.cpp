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
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
	//poco efficiente fare questo calcolo ogni volta 
	//o lo fai una volta all'inizio o lo metti come attributo
	ETileStatus Enemy;
	double OneXStep = 0;
	double TwoXStep = 0;
	double Xposition = PlaceAt.X;
	double Yposition = PlaceAt.Y;
	int32 OneStep = 0;
	TArray<ATile*> validMovesChoices;

	if (this->PieceColor == EColor::BLACK)
	{
		Enemy = ETileStatus::WHITEPIECE;
		OneXStep = PlaceAt.X - 1;
		TwoXStep = PlaceAt.X - 2;
		OneStep = -1;
	}
	else if (this->PieceColor == EColor::WHITE)
	{
		Enemy = ETileStatus::BLACKPIECE;
		OneXStep = PlaceAt.X + 1;
		TwoXStep = PlaceAt.X + 2;
		OneStep = 1;
	}

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

int32 APawnPiece::PieceWeight()
{
	return 1;
}

AChessPiece* APawnPiece::doVirtualMove(AChessPiece* Piece, ATile* from, ATile* to)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("PAWN DOVIRTUALMOVE"));
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	ETileStatus MyType = ETileStatus::EMPTY;
	ETileStatus OpponentType = ETileStatus::EMPTY;
	FVector2D MoveCurrPieceTo = to->GetGridPosition();
	AChessPiece* CapturedPiece = nullptr;
	TArray<AChessPiece*> OpponentPieceOnBoard = {};
	int32 OneStep = 0;

	if (Piece->PieceColor == EColor::BLACK)
	{
		MyType = ETileStatus::BLACKPIECE; OpponentType = ETileStatus::WHITEPIECE; OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard; OneStep = -1;
	}
	else if (Piece->PieceColor == EColor::WHITE)
	{
		MyType = ETileStatus::WHITEPIECE; OpponentType = ETileStatus::BLACKPIECE; OpponentPieceOnBoard = GameMode->ChessBoard->BlackPieceOnChessBoard; OneStep = 1;
	}

	/*EN PASSANT*/
	if (to->GetTileStatus() == ETileStatus::EMPTY && to->GetGridPosition().Y != from->GetGridPosition().Y)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("DOVIRTUALMOVE: EN PASSANT"));
		FVector2D OpponentPawnPosition = FVector2D(to->GetGridPosition().X - OneStep, to->GetGridPosition().Y);
		int32 Size = OpponentPieceOnBoard.Num();
		for (int32 i = 0; i < Size; i++)
		{
			if (OpponentPieceOnBoard[i]->PlaceAt == OpponentPawnPosition)
			{
				CapturedPiece = OpponentPieceOnBoard[i];
				break;
			}
		}

		GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(ETileStatus::EMPTY);
	}
	/*CATTURA DI UN PEZZO*/
	else if (to->GetTileStatus() == OpponentType)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("DOVIRTUALMOVE: CATTURA"));
		int32 Size = OpponentPieceOnBoard.Num();
		for (int32 i = 0; i < Size; i++)
		{
			if (OpponentPieceOnBoard[i]->PlaceAt == MoveCurrPieceTo)
			{
				CapturedPiece = OpponentPieceOnBoard[i];
				break;
			}
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

	from->SetTileStatus(ETileStatus::EMPTY);
	to->SetTileStatus(MyType);
	Piece->PlaceAt = MoveCurrPieceTo;

	return CapturedPiece;
}

void APawnPiece::undoVirtualMove(AChessPiece* Piece, ATile* from, ATile* to, AChessPiece* CapturedPiece)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("PAWN UNDDOVIRTUALMOVE"));
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	
	ETileStatus MyType = ETileStatus::EMPTY;
	ETileStatus OpponentType = ETileStatus::EMPTY;
	int32 OneStep = 0;

	if (Piece->PieceColor == EColor::BLACK)
	{
		MyType = ETileStatus::BLACKPIECE; OpponentType = ETileStatus::WHITEPIECE; OneStep = -1;
	}
	else if (Piece->PieceColor == EColor::WHITE)
	{
		MyType = ETileStatus::WHITEPIECE; OpponentType = ETileStatus::BLACKPIECE; OneStep = 1;
	}

	if (CapturedPiece != nullptr)
	{
		/*EN PASSANT*/
		if (CapturedPiece->PlaceAt.X != Piece->PlaceAt.X)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UNDDOVIRTUALMOVE EN PASSANT"));
			from->SetTileStatus(ETileStatus::EMPTY);
			FVector2D OpponentPawnPosition = FVector2D(from->GetGridPosition().X - OneStep, from->GetGridPosition().Y);
			GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(OpponentType);
			CapturedPiece->PlaceAt = OpponentPawnPosition;
		}
		/*CATTURA STANDARD*/
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UNDDOVIRTUALMOVE CATTURA"));
			from->SetTileStatus(OpponentType);
			CapturedPiece->PlaceAt = from->GetGridPosition();
		}

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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("UNDDOVIRTUALMOVE NORMALE"));
		from->SetTileStatus(ETileStatus::EMPTY);
	}	

	to->SetTileStatus(MyType);
	Piece->PlaceAt = to->GetGridPosition();
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
