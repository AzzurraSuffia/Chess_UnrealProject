// Fill out your copyright notice in the Description page of Project Settings.

#include "GameField.h"
#include "Chess_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Chess_GameMode.h"
#include "Chess_HumanPlayer.h"
#include "Chess_PlayerController.h"
#include "Chess_RandomPlayer.h"

const FRotator RotationPiece = FRotator(0.0f, 90.0f, 0.0f);

void ChangeTileStatus(EColor Color, const double InX, const double InY, AGameField* PlaySpace) {
	if (Color == EColor::WHITE)
	{
		PlaySpace->TileMap[FVector2D(InX, InY)]->SetTileStatus(ETileStatus::WHITEPIECE);
	}
	else if (Color == EColor::BLACK)
	{
		PlaySpace->TileMap[FVector2D(InX, InY)]->SetTileStatus(ETileStatus::BLACKPIECE);
	}
}

ARookPiece* AGameField::SpawnPieceRook(EColor Color, const double InX, const double InY, const float TileScale) {
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	//FRotator Rotation = ChooseRotation(Color);
	ChangeTileStatus(Color, InX, InY, this);
	ARookPiece* Obj = GetWorld()->SpawnActor<ARookPiece>(RookClass, Location, RotationPiece);
	const float PieceScale = (TileScale * 0.8);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);
	return Obj;
}

AKnightPiece* AGameField::SpawnPieceKnight(EColor Color, const double InX, const double InY, const float TileScale) {
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	//FRotator Rotation = ChooseRotation(Color);
	ChangeTileStatus(Color, InX, InY, this);
	AKnightPiece* Obj = GetWorld()->SpawnActor<AKnightPiece>(KnightClass, Location, RotationPiece);
	const float PieceScale = (TileScale * 0.8);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);
	return Obj;
}

ABishopPiece* AGameField::SpawnPieceBishop(EColor Color, const double InX, const double InY, const float TileScale) {
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	//FRotator Rotation = ChooseRotation(Color);
	ChangeTileStatus(Color, InX, InY, this);
	ABishopPiece* Obj = GetWorld()->SpawnActor<ABishopPiece>(BishopClass, Location, RotationPiece);
	const float PieceScale = (TileScale * 0.8);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);
	return Obj;
}

AQueenPiece* AGameField::SpawnPieceQueen(EColor Color, const double InX, const double InY, const float TileScale) {
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	//FRotator Rotation = ChooseRotation(Color);
	ChangeTileStatus(Color, InX, InY, this);
	AQueenPiece* Obj = GetWorld()->SpawnActor<AQueenPiece>(QueenClass, Location, RotationPiece);
	FVector Loggin = Obj->GetActorScale3D();
	//FVector OriginalScale = Obj0->GetActorScale3D();
	const float PieceScale = (TileScale * 0.8);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);
	return Obj;
}

AKingPiece* AGameField::SpawnPieceKing(EColor Color, const double InX, const double InY, const float TileScale) {
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	//FRotator Rotation = ChooseRotation(Color);
	ChangeTileStatus(Color, InX, InY, this);
	AKingPiece* Obj = GetWorld()->SpawnActor<AKingPiece>(KingClass, Location, RotationPiece);
	//FVector OriginalScale = Obj0->GetActorScale3D();
	const float PieceScale = (TileScale * 0.8);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);
	return Obj;
}

APawnPiece* AGameField::SpawnPiecePawn(EColor Color, const double InX, const double InY, const float TileScale) {
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	//FRotator Rotation = ChooseRotation(Color);
	ChangeTileStatus(Color, InX, InY, this);
	APawnPiece* Obj = GetWorld()->SpawnActor<APawnPiece>(PawnClass, Location, RotationPiece);
	const float PieceScale = (TileScale * 0.8);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);
	return Obj;
}

// Sets default values
AGameField::AGameField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/*VANNO NECESSARIAMENTE INIZIALIZZATI TUTTI GLI ATTRIBUTI?*/
	Size = 8;
	TileSize = 120;
	AGameField::WhiteKing = nullptr;
	AGameField::BlackKing = nullptr;
	AGameField::WhitePieceOnChessBoard = {};
	AGameField::BlackPieceOnChessBoard = {};
	//PieceSize = 120;
	// tile padding dimension
	// CellPadding = 20;

}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//normalized tilepadding
	//RoundToDouble converts a double to the nearest integer. Rounds up when the fraction is .5 
	// NormalizedCellPadding = FMath::RoundToDouble(((TileSize + CellPadding) / TileSize) * 100) / 100;
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
}

void DisplayPiecesStartConfiguration(AGameField* GField)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("DisplayStartConfiguration"));
	const float TileScale = GField->TileSize / 100;
	// Pieces creation
	for (int32 y = 0; y < GField->Size; y++)
	{
		//white pawns
		GField->WhitePieceOnChessBoard.Add(GField->SpawnPiecePawn(EColor::WHITE, 1, y, TileScale));

		//black pawns
		GField->BlackPieceOnChessBoard.Add(GField->SpawnPiecePawn(EColor::BLACK, GField->Size - 2, y, TileScale));
	}

	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceRook(EColor::WHITE, 0, 0, TileScale));
	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceRook(EColor::WHITE, 0, GField->Size - 1, TileScale));
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceRook(EColor::BLACK, GField->Size - 1, 0, TileScale));
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceRook(EColor::BLACK, GField->Size - 1, GField->Size - 1, TileScale));

	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceKnight(EColor::WHITE, 0, 1, TileScale));
	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceKnight(EColor::WHITE, 0, GField->Size - 2, TileScale));
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceKnight(EColor::BLACK, GField->Size - 1, 1, TileScale));
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceKnight(EColor::BLACK, GField->Size - 1, GField->Size - 2, TileScale));

	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceBishop(EColor::WHITE, 0, 2, TileScale));
	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceBishop(EColor::WHITE, 0, GField->Size - 3, TileScale));
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceBishop(EColor::BLACK, GField->Size - 1, 2, TileScale));
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceBishop(EColor::BLACK, GField->Size - 1, GField->Size - 3, TileScale));

	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceQueen(EColor::WHITE, 0, 3, TileScale));
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceQueen(EColor::BLACK, GField->Size - 1, GField->Size - 5, TileScale));

	AKingPiece* WKing = GField->SpawnPieceKing(EColor::WHITE, 0, 4, TileScale);
	GField->WhitePieceOnChessBoard.Add(WKing);
	GField->WhiteKing = WKing;
	AKingPiece* BKing = GField->SpawnPieceKing(EColor::BLACK, GField->Size - 1, GField->Size - 4, TileScale);
	GField->BlackPieceOnChessBoard.Add(BKing);
	GField->BlackKing = BKing;
}

void AGameField::ResetField()
{
	/*DEVO RIPORTARE IL CAMPO DA GIOCO ALLO STATO ORIGINALE*/
	/*PER INIZIALIZZARLO E RESETTERARLO POSSO CREARE UNA FUNZIONE DISPLAYSTARTPIECES()*/
	/*
	int32 NumWhitePieces = WhitePieceOnChessBoard.Num();
	for (int32 i = 0; i < NumWhitePieces; i++)
	{
		WhitePieceOnChessBoard[i]->Destroy();
		WhitePieceOnChessBoard.Remove(WhitePieceOnChessBoard[i]);
	}

	int32 NumBlackPieces = BlackPieceOnChessBoard.Num();
	for (int32 i = 0; i < NumBlackPieces; i++)
	{
		BlackPieceOnChessBoard[i]->Destroy();
		BlackPieceOnChessBoard.Remove(BlackPieceOnChessBoard[i]);
	}
	*/
	
	for (AChessPiece* WhitePiece : WhitePieceOnChessBoard)
	{
		TileMap[WhitePiece->PlaceAt]->SetTileStatus(ETileStatus::EMPTY);
		WhitePiece->Destroy();
	}
	WhitePieceOnChessBoard.Empty();

	for (AChessPiece* BlackPiece : BlackPieceOnChessBoard)
	{
		TileMap[BlackPiece->PlaceAt]->SetTileStatus(ETileStatus::EMPTY);
		BlackPiece->Destroy();
	}
	BlackPieceOnChessBoard.Empty();
	
	//non solo al checkmate le caselle dei re cambiano colore,
	//ma se clicco su reset dopo aver selezionato una tile è difficile risalire a quale fosse
	//finchè non istanzio la classe mossa
	RestoreSquareColor(TileArray);

	if (WhitePieceOnChessBoard.IsEmpty() && BlackPieceOnChessBoard.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ChessBoardEmpty"));
		DisplayPiecesStartConfiguration(this);
	}

	// send broadcast event to registered objects 
	OnResetEvent.Broadcast();

	AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PlayerController))
	{
		for (UUI_MoveBox* MoveBox : PlayerController->HUDChess->AllMoves)
		{
			if (MoveBox->Move->PieceCaptured != nullptr)
			{
				MoveBox->Move->PieceCaptured->Destroy();
			}
			if (MoveBox->Move->OriginalPawn != nullptr)
			{
				MoveBox->Move->OriginalPawn->Destroy();
			}

			MoveBox->ConditionalBeginDestroy();
		}
		PlayerController->HUDChess->AllMoves.Empty();

		for (UUI_TextBox* TextBox : PlayerController->HUDChess->OtherNotationComponents)
		{
			TextBox->ConditionalBeginDestroy();
		}
		PlayerController->HUDChess->OtherNotationComponents.Empty();
	}

	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->bisGameOver = false;
	GameMode->MoveCounter = 0;
	GameMode->ChoosePlayerAndStartGame();
}

FString AGameField::GetLastMoveAlgebricNotation()
{
	return MoveStack.Last()->AlgebricMoveNotation();
}

void AGameField::GenerateField()
{
	const float TileScale = TileSize / 100;

	// Chessboard creation
	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y);
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(x, y);
			if (((x+y) % 2) == 0)
			{
				Obj->SetTileColor(0);
			}
			else
			{
				Obj->SetTileColor(1);
			}

			TileArray.Add(Obj);
			TileMap.Add(FVector2D(x, y), Obj);
		}
	}

	DisplayPiecesStartConfiguration(this);
}

/* QUANDO USI GET POSITION RICORDADI USARE ISVALID PER VERIFICARE CHE IL CAST SIA ANDATO A BUON FINE */
FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	//restituisce l'attore toccato (se ne   stato toccato uno), ne fa il cast a Tile e ne restituisce la posizione x e y. 
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
	return TileArray;
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	//return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
	return TileSize * FVector(InX, InY, 0);
}

FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double x = Location[0] / (TileSize);
	const double y = Location[1] / (TileSize);
	return FVector2D(x, y);
}

void AGameField::PromotePawn(EPieceNotation ToPromote)
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	AChessPiece* Pawn = nullptr;
	int32 XChessBoardEdge = -1;
	TArray<AChessPiece*> PiecesOnChessBoard = {};
	EColor PColor = EColor::EMPTY;

	if (GameMode->CurrentPlayer == 0)
	{
		PiecesOnChessBoard = WhitePieceOnChessBoard;
		XChessBoardEdge = Size - 1;
		PColor = EColor::WHITE;
	}
	else if (GameMode->CurrentPlayer == 1)
	{
		PiecesOnChessBoard = BlackPieceOnChessBoard;
		XChessBoardEdge = 0;
		PColor = EColor::BLACK;
	}

	for (AChessPiece* CandidatePawn : PiecesOnChessBoard)
	{
		if (CandidatePawn->PlaceAt.X == XChessBoardEdge)
		{
			AChessPiece* PawnReference = Cast<APawnPiece>(CandidatePawn);
			if (IsValid(PawnReference))
			{
				Pawn = PawnReference;
				break;
			}
		}
	}

	FVector2D SpawnPosition = Pawn->PlaceAt;
	const float TileScale = TileSize / 100;
	//Pawn->SetActorHiddenInGame(true);
	MoveStack.Last()->OriginalPawn = Pawn;
	
	MoveOutOfChessBoard(Pawn, true);

	if (ToPromote != EPieceNotation::EMPTY && Pawn != nullptr)
	{
		AChessPiece* PiecePromoted;
		switch (ToPromote)
		{
		case EPieceNotation::B:
			PiecePromoted = SpawnPieceBishop(PColor, SpawnPosition.X, SpawnPosition.Y, TileScale);
			break;
		case EPieceNotation::N:
			PiecePromoted = SpawnPieceKnight(PColor, SpawnPosition.X, SpawnPosition.Y, TileScale);
			break;
		case EPieceNotation::R:
			PiecePromoted = SpawnPieceRook(PColor, SpawnPosition.X, SpawnPosition.Y, TileScale);
			break;
		case EPieceNotation::Q:
			PiecePromoted = SpawnPieceQueen(PColor, SpawnPosition.X, SpawnPosition.Y, TileScale);
			break;
		default:
			PiecePromoted = SpawnPieceQueen(PColor, SpawnPosition.X, SpawnPosition.Y, TileScale);
		}

		MoveStack.Last()->PiecePromoted = PiecePromoted;

		if (GameMode->CurrentPlayer == 0)
		{
			WhitePieceOnChessBoard.Remove(Pawn);
			WhitePieceOnChessBoard.Add(PiecePromoted);
			GameMode->Players[GameMode->CurrentPlayer]->OnPawnPromotion();
		}
		else if (GameMode->CurrentPlayer == 1)
		{
			BlackPieceOnChessBoard.Remove(Pawn);
			BlackPieceOnChessBoard.Add(PiecePromoted);
		}
	}
}

void AGameField::MoveOutOfChessBoard(AChessPiece* HiddenPiece, bool flag)
{
	int32 CellPosition = 0;
	int32 Xposition = 0;

	if (HiddenPiece->PieceColor == EColor::WHITE)
	{
		CellPosition = 16 - WhitePieceOnChessBoard.Num() - 1;
		Xposition = -1;
		if (CellPosition > 8)
		{
			Xposition = -2;
		}
		else if (CellPosition > 16)
		{
			Xposition = -3;
		}
	}
	else if (HiddenPiece->PieceColor == EColor::BLACK)
	{
		CellPosition = 16 - BlackPieceOnChessBoard.Num() - 1;
		Xposition = 8;
		if (CellPosition > 8)
		{
			Xposition = 9;
		}
		else if (CellPosition > 16)
		{
			Xposition = 10;
		}
	}
	
	int32 Yposition = CellPosition % Size;

	if (flag)
	{
		Yposition++;
	}

	FVector Location = GetRelativeLocationByXYPosition(Xposition, Yposition);
	FVector NewLocation = Location + FVector(6, 6, 20);
	HiddenPiece->SetActorLocation(NewLocation);
	HiddenPiece->PlaceAt = FVector2D(Xposition, Yposition);
}

void AGameField::RestoreSquareColor(TArray<ATile*> Squares)
{
	for (ATile* Tile : Squares)
	{
		int32 x = Tile->GetGridPosition().X;
		int32 y = Tile->GetGridPosition().Y;
		if(((x + y) % 2) == 0)
		{
			Tile->SetTileColor(0);
		}
		else
		{
			Tile->SetTileColor(1);
		}
	}
	
}
