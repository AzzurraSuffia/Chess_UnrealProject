// Fill out your copyright notice in the Description page of Project Settings.

#include "GameField.h"
#include "Chess_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Chess_GameMode.h"
#include "Chess_HumanPlayer.h"
#include "Chess_PlayerController.h"
#include "Chess_RandomPlayer.h"

const FRotator RotationPiece = FRotator(0.0f, 90.0f, 0.0f);

//set the tile as occupied based on the color passed as an argument
void ChangeTileStatus(EColor Color, const double InX, const double InY, AGameField* PlaySpace) {
	if (Color == EColor::WHITE && PlaySpace->TileMap.Contains(FVector2D(InX, InY)))
	{
		PlaySpace->TileMap[FVector2D(InX, InY)]->SetTileStatus(ETileStatus::WHITEPIECE);
	}
	else if (Color == EColor::BLACK && PlaySpace->TileMap.Contains(FVector2D(InX, InY)))
	{
		PlaySpace->TileMap[FVector2D(InX, InY)]->SetTileStatus(ETileStatus::BLACKPIECE);
	}
}


ARookPiece* AGameField::SpawnPieceRook(EColor Color, const double InX, const double InY, const float TileScale) {

	//spawn a rook in the location passed as argument
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	ChangeTileStatus(Color, InX, InY, this);
	ARookPiece* Obj = GetWorld()->SpawnActor<ARookPiece>(RookClass, Location, RotationPiece);

	//set piece scale
	const float PieceScale = (TileScale * 0.7);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));

	//set piece attributes
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);

	return Obj;
}

AKnightPiece* AGameField::SpawnPieceKnight(EColor Color, const double InX, const double InY, const float TileScale) {

	//spawn a knight in the location passed as argument
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	ChangeTileStatus(Color, InX, InY, this);
	AKnightPiece* Obj = GetWorld()->SpawnActor<AKnightPiece>(KnightClass, Location, RotationPiece);

	//set piece scale
	const float PieceScale = (TileScale * 0.7);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));

	//set piece attributes
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);
	return Obj;
}

ABishopPiece* AGameField::SpawnPieceBishop(EColor Color, const double InX, const double InY, const float TileScale) {

	//spawn a bishop in the location passed as argument
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	ChangeTileStatus(Color, InX, InY, this);
	ABishopPiece* Obj = GetWorld()->SpawnActor<ABishopPiece>(BishopClass, Location, RotationPiece);

	//set piece scale
	const float PieceScale = (TileScale * 0.7);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));

	//set piece attributes
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);
	return Obj;
}

AQueenPiece* AGameField::SpawnPieceQueen(EColor Color, const double InX, const double InY, const float TileScale) {

	//spawn a queen in the location passed as argument
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	ChangeTileStatus(Color, InX, InY, this);
	AQueenPiece* Obj = GetWorld()->SpawnActor<AQueenPiece>(QueenClass, Location, RotationPiece);

	//set piece scale
	const float PieceScale = (TileScale * 0.7);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));

	//set piece attributes
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);
	return Obj;
}

AKingPiece* AGameField::SpawnPieceKing(EColor Color, const double InX, const double InY, const float TileScale) {

	//spawn a king in the location passed as argument
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale*5, TileScale*5, 20));
	ChangeTileStatus(Color, InX, InY, this);
	AKingPiece* Obj = GetWorld()->SpawnActor<AKingPiece>(KingClass, Location, RotationPiece);

	//set piece scale
	const float PieceScale = (TileScale * 0.7);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));

	//set piece attributes
	Obj->PieceColor = Color;
	Obj->ChessBoard = this;
	Obj->PlaceAt = FVector2D(InX, InY);
	Obj->SetPieceColor(Color);
	return Obj;
}

APawnPiece* AGameField::SpawnPiecePawn(EColor Color, const double InX, const double InY, const float TileScale) {

	//spawn a pawn in the location passed as argument
	FVector Location = (AGameField::GetRelativeLocationByXYPosition(InX, InY) + FVector(TileScale * 5, TileScale * 5, 20));
	ChangeTileStatus(Color, InX, InY, this);
	APawnPiece* Obj = GetWorld()->SpawnActor<APawnPiece>(PawnClass, Location, RotationPiece);

	//set piece scale
	const float PieceScale = (TileScale * 0.7);
	Obj->SetActorScale3D(FVector(PieceScale, PieceScale, 0.2));

	//set piece attributes
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

	Size = 8;
	TileSize = 120;
	AGameField::WhiteKing = nullptr;
	AGameField::BlackKing = nullptr;
	AGameField::WhitePieceOnChessBoard = {};
	AGameField::BlackPieceOnChessBoard = {};


}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
}

//spawn all pieces and display them in chess start configuration
void DisplayPiecesStartConfiguration(AGameField* GField)
{
	const float TileScale = GField->TileSize / 100;
	
	for (int32 y = 0; y < GField->Size; y++)
	{
		//spawn white pawn and add it to white pieces on chessboard
		GField->WhitePieceOnChessBoard.Add(GField->SpawnPiecePawn(EColor::WHITE, 1, y, TileScale));

		//spawn black pawn and add it to black pieces on chessboard
		GField->BlackPieceOnChessBoard.Add(GField->SpawnPiecePawn(EColor::BLACK, GField->Size - 2, y, TileScale));
	}

	//spawn white rooks and add them to white pieces on chessboard
	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceRook(EColor::WHITE, 0, 0, TileScale));
	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceRook(EColor::WHITE, 0, GField->Size - 1, TileScale));

	//spawn black rooks and add them to black pieces on chessboard
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceRook(EColor::BLACK, GField->Size - 1, 0, TileScale));
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceRook(EColor::BLACK, GField->Size - 1, GField->Size - 1, TileScale));
	
	//spawn white knights and add them to white pieces on chessboard
	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceKnight(EColor::WHITE, 0, 1, TileScale));
	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceKnight(EColor::WHITE, 0, GField->Size - 2, TileScale));

	//spawn black knights and add them to black pieces on chessboard
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceKnight(EColor::BLACK, GField->Size - 1, 1, TileScale));
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceKnight(EColor::BLACK, GField->Size - 1, GField->Size - 2, TileScale));
	
	//spawn white bishops and add them to white pieces on chessboard
	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceBishop(EColor::WHITE, 0, 2, TileScale));
	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceBishop(EColor::WHITE, 0, GField->Size - 3, TileScale));

	//spawn black bishops and add them to black pieces on chessboard
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceBishop(EColor::BLACK, GField->Size - 1, 2, TileScale));
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceBishop(EColor::BLACK, GField->Size - 1, GField->Size - 3, TileScale));
	
	//spawn white queen and add it to white pieces on chessboard
	GField->WhitePieceOnChessBoard.Add(GField->SpawnPieceQueen(EColor::WHITE, 0, 3, TileScale));

	//spawn black queen and add it to black pieces on chessboard
	GField->BlackPieceOnChessBoard.Add(GField->SpawnPieceQueen(EColor::BLACK, GField->Size - 1, GField->Size - 5, TileScale));
	
	//spawn white king and add it to white pieces on chessboard
	AKingPiece* WKing = GField->SpawnPieceKing(EColor::WHITE, 0, 4, TileScale);
	GField->WhitePieceOnChessBoard.Add(WKing);
	GField->WhiteKing = WKing;

	//spawn black king and add it to black pieces on chessboard
	AKingPiece* BKing = GField->SpawnPieceKing(EColor::BLACK, GField->Size - 1, GField->Size - 4, TileScale);
	GField->BlackPieceOnChessBoard.Add(BKing);
	GField->BlackKing = BKing;
}

void AGameField::ResetField()
{
	//destroy each white piece on chessboard
	for (AChessPiece* WhitePiece : WhitePieceOnChessBoard)
	{
		TileMap[WhitePiece->PlaceAt]->SetTileStatus(ETileStatus::EMPTY);
		WhitePiece->Destroy();
	}

	//empty the array
	WhitePieceOnChessBoard.Empty();

	//destroy each black piece on chessboard
	for (AChessPiece* BlackPiece : BlackPieceOnChessBoard)
	{
		TileMap[BlackPiece->PlaceAt]->SetTileStatus(ETileStatus::EMPTY);
		BlackPiece->Destroy();
	}

	//empty the array
	BlackPieceOnChessBoard.Empty();
	
	RestoreSquaresColor(TileArray);

	//send broadcast event to registered objects 
	OnResetEvent.Broadcast();

	CurrentChessboardState = nullptr;

	//destroy each move in move stack
	for (UMove* Move : MoveStack)
	{
		Move->ConditionalBeginDestroy();
	}

	//empty the array
	MoveStack.Empty();

	//spawn all pieces and display them in chess start configuration
	DisplayPiecesStartConfiguration(this);

	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->bisGameOver = false;
	GameMode->MoveCounter = 0;

	AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PlayerController))
	{
		for (UUI_MoveBox* MoveBox : PlayerController->HUDChess->AllMoves)
		{
			//destroy all hidden pieces out of chessboard unrolling the storyboard
			if (MoveBox->Move->bisCapture && MoveBox->Move->PieceCaptured != nullptr)
			{
				MoveBox->Move->PieceCaptured->Destroy();
			}
			if (MoveBox->Move->benPassant && MoveBox->Move->PieceCaptured != nullptr)
			{
				MoveBox->Move->PieceCaptured->Destroy();
			}
			if (MoveBox->Move->bisPromotion)
			{
				MoveBox->Move->PieceMoving->Destroy();
			}

			//destroy all move buttons in the storyboard
			MoveBox->ConditionalBeginDestroy();
		}

		//empty the array
		PlayerController->HUDChess->AllMoves.Empty();

		//destroy all text boxes in the storyboard
		for (UUI_TextBox* TextBox : PlayerController->HUDChess->OtherNotationComponents)
		{
			TextBox->ConditionalBeginDestroy();
		}

		//empty the array
		PlayerController->HUDChess->OtherNotationComponents.Empty();
	}

	//show the start menu
	GameMode->OnStart.Broadcast();
}

void AGameField::GenerateField()
{
	const float TileScale = TileSize / 100;
	TArray<FString> Number2Letter = { FString("a"), FString("b"), FString("c"), FString("d"), FString("e"), FString("f"), FString("g"), FString("h") };

	//chessboard creation
	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			//spawn the tile
			FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y);
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));

			//set tile position and color
			Obj->SetGridPosition(x, y);
			if (((x+y) % 2) == 0)
			{
				Obj->SetTileColor(0);

				//set text number component and text letter component colors for some tiles
				if (y == 0)
				{
					Obj->SetTextColorHex(Obj->GetTextNumber(), "F5F5DC");
				}
				if (x == 0)
				{
					Obj->SetTextColorHex(Obj->GetTextLetter(), "F5F5DC");
				}

			}
			else
			{
				Obj->SetTileColor(1);

				//set text number component and text letter component colors for some tiles
				if (y == 0)
				{
					Obj->SetTextColorHex(Obj->GetTextNumber(), "001a00");
				}
				if (x == 0)
				{
					Obj->SetTextColorHex(Obj->GetTextLetter(), "001a00");
				}
			}

			//set text number component and text letter component for some tiles
			if (y == 0)
			{
				Obj->SetTileNumber(x+1);
			}
			if (x == 0)
			{
				Obj->SetTileLetter(Number2Letter[y]);
			}

			TileArray.Add(Obj);
			TileMap.Add(FVector2D(x, y), Obj);
		}
	}

	//spawn all pieces and display them in chess start configuration
	DisplayPiecesStartConfiguration(this);
}

FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
	return TileArray;
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
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

	//based on the player, define his pieces, his opposite edge and color
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

	//get the reference to the pawn to promote
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

	//hide the pawn and move it out of chessboard
	FVector2D SpawnPosition = Pawn->PlaceAt;
	const float TileScale = TileSize / 100;
	Pawn->SetActorHiddenInGame(true);
	MoveOutOfChessBoard(Pawn);

	//based on the argument, spawn a new different piece to replace the pawn
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

		//update this move with promotion
		MoveStack.Last()->PiecePromoted = PiecePromoted;

		//remove the pawn from chessboard and add the new piece instead
		if (GameMode->CurrentPlayer == 0)
		{	
			WhitePieceOnChessBoard.Remove(Pawn);
			WhitePieceOnChessBoard.Add(PiecePromoted);

			//this call ends human pawn promotion
			GameMode->Players[GameMode->CurrentPlayer]->OnPawnPromotion();
		}
		else if (GameMode->CurrentPlayer == 1)
		{
			BlackPieceOnChessBoard.Remove(Pawn);
			BlackPieceOnChessBoard.Add(PiecePromoted);
		}
	}
}

void AGameField::MoveOutOfChessBoard(AChessPiece* HiddenPiece)
{
	int32 CellPosition = 0;
	int32 Xposition = 0;

	//basedon piece color set its position out of chessboard
	if (HiddenPiece->PieceColor == EColor::WHITE)
	{
		CellPosition = 15 - WhitePieceOnChessBoard.Num();
		Xposition = -1;

		//if there are yet 8 pieces out of chessboard 
		if (CellPosition > 8)
		{
			Xposition = -2;
		}
		//if there are yet 16 pieces out of chessboard 
		else if (CellPosition > 16)
		{
			Xposition = -3;
		}
		
	}
	else if (HiddenPiece->PieceColor == EColor::BLACK)
	{
		CellPosition = 15 - BlackPieceOnChessBoard.Num();
		Xposition = 8;

		//if there are yet 8 pieces out of chessboard 
		if (CellPosition > 8)
		{
			Xposition = 9;
		}
		//if there are yet 16 pieces out of chessboard 
		else if (CellPosition > 16)
		{
			Xposition = 10;
		}
	}
	
	int32 Yposition = CellPosition % Size;

	//move the piece
	FVector Location = GetRelativeLocationByXYPosition(Xposition, Yposition);
	FVector NewLocation = Location + FVector(6, 6, 20);
	HiddenPiece->SetActorLocation(NewLocation);
}

void AGameField::RestoreSquaresColor(TArray<ATile*> Squares)
{
	for (ATile* Tile : Squares)
	{
		RestoreASquareColor(Tile);
	}
}

void AGameField::RestoreASquareColor(ATile* Square)
{
	int32 x = Square->GetGridPosition().X;
	int32 y = Square->GetGridPosition().Y;
	if (((x + y) % 2) == 0)
	{
		Square->SetTileColor(0);
	}
	else
	{
		Square->SetTileColor(1);
	}
}