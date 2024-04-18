// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_GameMode.h"
#include "Chess_PlayerController.h"
#include "Chess_HumanPlayer.h"
#include "Chess_RandomPlayer.h"
#include "Chess_MinimaxPlayer.h"
#include "Math/UnrealMathUtility.h"
#include "EngineUtils.h"

bool AChess_GameMode::GetPromotionFlag() const
{
	return bpromotionFlag;
}

int32 AChess_GameMode::GetMoveCounter() const
{
	return MoveCounter;
}

bool AChess_GameMode::IsGameEnded(UMove* Move, AKingPiece* King)
{
	int32 NextPlayer = GetNextPlayer(CurrentPlayer);
	
	if (PlayerCanMove(NextPlayer))
	{
		if (IsKingInCheck(NextPlayer))
		{
			Move->bisCheck = true;
			ChessBoard->TileMap[King->PlaceAt]->SetTileColor(4);
		}
		return false;
	}
	else
	{
		//la partita è finita patta o ha vinto l'avversario
		if (IsKingInCheck(NextPlayer))
		{
			Move->bisCheckmate = true;
			ChessBoard->TileMap[King->PlaceAt]->SetTileColor(4);
			ManageEndOfGame(CurrentPlayer, EResult::CHECKMATE);
		}
		else
		{
			ManageEndOfGame(CurrentPlayer, EResult::STALEMATE);
		}
		return true;
	}
}

bool AChess_GameMode::IsKingInCheck(int32 Player)
{
	AKingPiece* MyKing = nullptr;
	TArray<AChessPiece*> OpponentPieces = {};
	if (Player == 0)
	{
		/*WHITE KING*/
		MyKing = ChessBoard->WhiteKing;
		OpponentPieces = ChessBoard->BlackPieceOnChessBoard;
		
	}
	else if (Player == 1)
	{
		/*BLACK KING*/
		MyKing = ChessBoard->BlackKing;
		OpponentPieces = ChessBoard->WhitePieceOnChessBoard;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("IsKingInCheck"));
	if (!OpponentPieces.IsEmpty())
	{
		for (AChessPiece* Piece : OpponentPieces)
		{
			if (Piece != nullptr)
			{
				if (Piece->CanCaptureOpponentPiece(MyKing))
				{
					return true;
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("GameMode.cpp: PIECE VUOTO NELL'ARRAY"));
			}
		}
	}
	
	return false;
}

void AChess_GameMode::ManageEndOfGame(int32 Player, EResult GameResult)
{
	bisGameOver = true;
	if (GameResult == EResult::CHECKMATE)
	{
		Players[Player]->OnWin();
		for (int32 i = 0; i < Players.Num(); i++)
		{
			if (i != Player)
			{
				Players[i]->OnLose();
			}
		}

		FString WinningString = FString();
		(Player == 0) ? WinningString = FString("1-0") : WinningString = FString("0-1");
		AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
		if (IsValid(PlayerController))
		{
			int32 NumberOfMoves = PlayerController->HUDChess->AllMoves.Num();
			float Xposition = 0.0f;
			float Yposition = (NumberOfMoves / 2) * 50.0f + 50.0f;
			PlayerController->HUDChess->AddTextWidget(WinningString,FVector2D(Xposition, Yposition), FVector2D(200.0f, 50.0f));
			PlayerController->HUDChess->ResetButtonWidget->SetIsEnabled(true);
		}
	}
	else if (GameResult == EResult::STALEMATE)
	{
		for (int32 i = 0; i < Players.Num(); i++)
		{
			Players[i]->OnDraw(EResult::STALEMATE);
		}

		AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
		if (IsValid(PlayerController))
		{
			int32 NumberOfMoves = PlayerController->HUDChess->AllMoves.Num();
			float Xposition = 0.0f;
			float Yposition = (NumberOfMoves / 2) * 50.0f + 50.0f;
			FString DrawString = FString("1/2-1/2");
			PlayerController->HUDChess->AddTextWidget(DrawString, FVector2D(Xposition, Yposition), FVector2D(200.0f, 50.0f));
		}
		
		// add a timer (3 seconds)
		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				// function to delay
				ChessBoard->ResetField();

			}, 3, false);
	}
}

bool AChess_GameMode::PlayerCanMove(int32 Player)
{
	TArray<AChessPiece*> PiecesOnChessBoard = {};
	if (Player == 0)
	{
		PiecesOnChessBoard = ChessBoard->WhitePieceOnChessBoard;
	}
	else if (Player == 1)
	{
		PiecesOnChessBoard = ChessBoard->BlackPieceOnChessBoard;
	}

	for (AChessPiece* Piece : PiecesOnChessBoard)
	{
		TArray<ATile*> candidateMoves = {};
		if (APawnPiece* Pawn = Cast<APawnPiece>(Piece))
		{
			bool mantainValue = Pawn->bfirstMove;
			candidateMoves = Pawn->validMoves();
			Pawn->bfirstMove = mantainValue;
		}
		else
		{
			candidateMoves = Piece->validMoves();
		}

		TArray<ATile*> actualMoves = {};
		for (ATile* validTile : candidateMoves)
		{
			if (Piece->IsLegal(validTile))
			{
				actualMoves.Add(validTile);
			}
		}

		if (!actualMoves.IsEmpty())
		{
			return true;
		}
	}

	return false;
}

bool AChess_GameMode::CheckForPawnPromotion(AChessPiece* CurrPiece)
{
		int32 XChessBoardEdge = (CurrPiece->PieceColor == EColor::WHITE) ? CurrPiece->ChessBoard->Size - 1 : 0;

		if (CurrPiece->PlaceAt.X == XChessBoardEdge)
		{
			APawnPiece* Pawn = Cast<APawnPiece>(CurrPiece);
			if (IsValid(Pawn))
			{
				ChessBoard->MoveStack.Last()->bisPromotion = true;
				/*PROMOZIONE DEL PEDONE*/
				Players[CurrentPlayer]->OnPawnPromotion();
				return true;
			}
		}
		return false;
}

void AChess_GameMode::MovePiece(AChessPiece* Piece, ATile* From, ATile* To)
{
	FVector2D MoveCurrPieceTo = To->GetGridPosition();
	FVector Location = ChessBoard->GetRelativeLocationByXYPosition(MoveCurrPieceTo.X, MoveCurrPieceTo.Y);
	FVector NewLocation = Location + FVector(6, 6, 20);
	Piece->SetActorLocation(NewLocation);
	Piece->PlaceAt = MoveCurrPieceTo;
	(Piece->PieceColor == EColor::WHITE) ? To->SetTileStatus(ETileStatus::WHITEPIECE) : To->SetTileStatus(ETileStatus::BLACKPIECE);
	From->SetTileStatus(ETileStatus::EMPTY);
}

/*
TArray<TArray<ATile*>>& AChess_GameMode::FindAllLegalMoves(int32 Player)
{
	//L'inizializzazione non va bene
	TArray<TArray<ATile*>>& AllLegalMoves = {};
	TArray<AChessPiece*> PiecesOnChessBoard = {};

	if (Player == 0)
	{
		PiecesOnChessBoard = this->WhitePieceOnChessBoard;
	}
	else if (Player == 1)
	{
		PiecesOnChessBoard = this->BlackPieceOnChessBoard;
	}

	for (AChessPiece* Piece : PiecesOnChessBoard)
	{
		TArray<ATile*> candidateMoves = Piece->validMoves();
		TArray<ATile*> actualMoves = {};
		for (ATile* validTile : candidateMoves)
		{
			if (Piece->IsLegal(validTile))
			{
				actualMoves.Add(validTile);
			}
		}
		AllLegalMoves.Add(actualMoves);
	}

	return AllLegalMoves;
}
*/

AChess_GameMode::AChess_GameMode()
{
	PlayerControllerClass = AChess_PlayerController::StaticClass();
	DefaultPawnClass = AChess_HumanPlayer::StaticClass();
	FieldSize = 8;
}

//quello che accade appena parte il gioco, quello che inizia il gioco
void AChess_GameMode::BeginPlay()
{
	Super::BeginPlay();

	bisGameOver = false;

	bpromotionFlag = false;

	MoveCounter = 0;

	AChess_HumanPlayer* HumanPlayer = Cast<AChess_HumanPlayer>(*TActorIterator<AChess_HumanPlayer>(GetWorld()));
	if (IsValid(HumanPlayer))
	{
		if (GameFieldClass != nullptr)
		{
			ChessBoard = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
			//poichè la generate field viene chiamata nella BeginPlay di GameField conil valore size settato nel costruttore di GameField
			// questo assegnamento è inutile, o si commenta questo o si toglie la generatefield dalla BeginPlay della GameField e la si mette qui 
			// in quest'ultimo modo genera un campo con dimensione FieldSize e non Size
			//GField->Size = FieldSize;
			//GField->GenerateField();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
			return;
		}

		
		//float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
		float CameraPosX = ((ChessBoard->TileSize * (FieldSize)) / 2) - (ChessBoard->TileSize / 2);
		FVector CameraPos(CameraPosX, CameraPosX, 1500.0f);
		HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());
		
		AllPlayers.Add(HumanPlayer);
		Players.Add(HumanPlayer);

		auto* AIRandom = GetWorld()->SpawnActor<AChess_RandomPlayer>(FVector(), FRotator());
		AllPlayers.Add(AIRandom);

		auto* AIMinimax = GetWorld()->SpawnActor<AChess_MinimaxPlayer>(FVector(), FRotator());
		AllPlayers.Add(AIMinimax);

	}
	//Va segnalato il fallimento di un cast?
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cast failed"));
	}
}

void AChess_GameMode::ChoosePlayerAndStartGame(bool difficult)
{
	//seleziona il giocatore casualmente tra l'array di Player (funzione generale, indipendente dal particolare numero di giocatori, ci piace)
	//CurrentPlayer = FMath::RandRange(0, Players.Num() - 1);
	
	//Nelle specifiche viene richiesto che ad iniziare sia sempre l'umano
	CurrentPlayer = 0;

	// AI player = 1
	if (difficult)
	{ 
		if (Players.Num() == 2)
		{
			Players[1] = AllPlayers[2];
		}
		else
		{
			Players.Add(AllPlayers[2]);
		}
	}
	else
	{
		if (Players.Num() == 2)
		{
			Players[1] = AllPlayers[1];
		}
		else
		{
			Players.Add(AllPlayers[1]);
		}
	}

	//assegnazone di un numero a ogni giocatore e di un colore delle pedine
	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
		Players[i]->Set = i == CurrentPlayer ? ESet::WHITE : ESet::BLACK;
	}

	MoveCounter += 1;
	//inizia il turno del giocatore estratto casualmente
	Players[CurrentPlayer]->OnTurn();

}

//ottengo il prossimo giocatore
int32 AChess_GameMode::GetNextPlayer(int32 Player)
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;

}

//assegno il turno al prossimo giocatore
void AChess_GameMode::TurnNextPlayer()
{
	CurrentPlayer = GetNextPlayer(CurrentPlayer);
	if (CurrentPlayer == 0)
	{
		MoveCounter += 1;
	}
	Players[CurrentPlayer]->OnTurn();
}

TArray<ATile*> AChess_GameMode::DetectEnPassant(APawnPiece* Pawn, AChessPiece* PreviousPieceMoving, ATile* PreviousTo, ATile* PreviousFrom)
{
	int32 OneStep = 0;
	int32 OneXStep = 0;
	int32 Yposition = Pawn->PlaceAt.Y;
	TArray<ATile*> EnPassantSquare = {};

	if (Pawn->PieceColor == EColor::WHITE)
	{
		OneStep = 1;
		OneXStep = Pawn->PlaceAt.X + 1;
	}
	else
	{
		OneStep = -1;
		OneXStep = Pawn->PlaceAt.X - 1;
	}

	TArray<FVector2D> DiagonalSquareLocations = { FVector2D(OneXStep, Yposition - 1), FVector2D(OneXStep, Yposition + 1) };

	for (FVector2D DiagonalSquareLocation : DiagonalSquareLocations)
	{
		if (ChessBoard->TileMap.Contains(DiagonalSquareLocation))
		{
			//controllo inutile DiagonalSquare->GetTileStatus() == ETileStatus::EMPTY perchè se l'ultima mossa è stataquella di un pedone
			// che ha mosso due la cella diero di sè è per forza libera. Posso usarlo per uscire prima se non sono in quel caso
			ATile* DiagonalSquare = ChessBoard->TileMap[DiagonalSquareLocation];
			if (DiagonalSquare->GetTileStatus() == ETileStatus::EMPTY)
			{
					//verifico che il pedone e piece moving siano affiancati e piecemoving sia sotto diagonalsquare
					if ((PreviousTo->GetGridPosition().X + OneStep) == DiagonalSquare->GetGridPosition().X && PreviousTo->GetGridPosition().Y == DiagonalSquare->GetGridPosition().Y)
					{
						//verifico che il pezzo a fianco sia un pedone
						APawnPiece* OpponentPawn = Cast<APawnPiece>(PreviousPieceMoving);
						if (IsValid(OpponentPawn))
						{
							//verifico che nel turno prima abbiamo mosso per la prima volta e di due passi
							int32 OpponentPawnXStartPosition = PreviousFrom->GetGridPosition().X;
							int32 OpponentPawnXEndPosition = PreviousTo->GetGridPosition().X;
							if (FMath::Abs(OpponentPawnXEndPosition - OpponentPawnXStartPosition) == 2)
							{
								GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("EN PASSANT"));
								EnPassantSquare.Add(DiagonalSquare);
							}
						}
					}
			}
		}
	}
	return EnPassantSquare;
}
