// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_GameMode.h"
#include "Chess_PlayerController.h"
#include "Chess_HumanPlayer.h"
#include "Chess_RandomPlayer.h"
#include "Chess_MinimaxPlayer.h"
#include "Math/UnrealMathUtility.h"
#include "EngineUtils.h"

bool AChess_GameMode::IsGameEnded(UMove* Move, AKingPiece* King)
{
	int32 NextPlayer = GetNextPlayer(CurrentPlayer);
	
	//if next player has some legal moves left, the game continues
	if (PlayerCanMove(NextPlayer))
	{
		//if next player's king is in check, keep track of check in the move object
		if (IsKingInCheck(NextPlayer))
		{
			Move->bisCheck = true;
		}
		return false;
	}
	//if next player has not any legal moves left, the game ends
	else
	{
		//if next player's king is in check, it's a checkmate and the other player wins
		if (IsKingInCheck(NextPlayer))
		{
			// keep track of checkmate in the move object
			Move->bisCheckmate = true;
			ChessBoard->TileMap[King->PlaceAt]->SetTileColor(4);
			ManageEndOfGame(CurrentPlayer, EResult::CHECKMATE);
		}

		//if next player's king is not in check, it's a stalemate and the game ends in a draw
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

	//based on the player, define what MyKing and OpponentPieces are
	if (Player == 0)
	{
		MyKing = ChessBoard->WhiteKing;
		OpponentPieces = ChessBoard->BlackPieceOnChessBoard;
		
	}
	else if (Player == 1)
	{
		MyKing = ChessBoard->BlackKing;
		OpponentPieces = ChessBoard->WhitePieceOnChessBoard;
	}

	//Opponent's pieces should never be empty, it's a secure major
	if (!OpponentPieces.IsEmpty())
	{
		//for each piece in opponent's piece check if it can capture the king
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
			}
		}
	}
	
	return false;
}

void AChess_GameMode::ManageEndOfGame(int32 Player, EResult GameResult)
{
	bisGameOver = true;

	//if the game ended in a checkmate, the last player who has moved wins and all others lose
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

		//choose winning string notation depending on the winning player
		FString WinningString = FString();
		(Player == 0) ? WinningString = FString("1-0") : WinningString = FString("0-1");
		AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
		if (IsValid(PlayerController))
		{
			int32 NumberOfMoves = PlayerController->HUDChess->AllMoves.Num();
			float Xposition = 0.0f;
			float Yposition = (NumberOfMoves / 2) * 50.0f + 50.0f;

			//add the winning string to the storyboard
			PlayerController->HUDChess->AddTextWidget(WinningString,FVector2D(Xposition, Yposition), FVector2D(200.0f, 50.0f));

			//enable move buttons and restart button 
			for (UUI_MoveBox* MoveBox : PlayerController->HUDChess->AllMoves)
			{
				MoveBox->SetIsEnabled(true);
			}
			PlayerController->HUDChess->ResetButtonWidget->SetIsEnabled(true);
		}
	}

	//if the game ended in a stalemate, it's a draw for everyone
	else if (GameResult == EResult::STALEMATE)
	{
		for (int32 i = 0; i < Players.Num(); i++)
		{
			Players[i]->OnDraw(EResult::STALEMATE);
		}

		AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
		if (IsValid(PlayerController))
		{
			//add the draw string to the storyboard
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
				// after a draw restart automatically the game
				ChessBoard->ResetField();

			}, 3, false);
	}
}

bool AChess_GameMode::PlayerCanMove(int32 Player)
{
	//based on the player, define what his pieces on chessboard are
	TArray<AChessPiece*> PiecesOnChessBoard = {};
	if (Player == 0)
	{
		PiecesOnChessBoard = ChessBoard->WhitePieceOnChessBoard;
	}
	else if (Player == 1)
	{
		PiecesOnChessBoard = ChessBoard->BlackPieceOnChessBoard;
	}

	//for each piece check if there are legal moves
	for (AChessPiece* Piece : PiecesOnChessBoard)
	{
		TArray<ATile*> candidateMoves = {};

		//if it's a pawn do not alter the first turn boolean
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

		//look for legal moves
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
			//if there's at least one piece with possible legal moves the player can move
			return true;
		}
	}

	//if there's not at least one piece with possible legal moves the player cannot move
	return false;
}

bool AChess_GameMode::CheckForPawnPromotion(AChessPiece* CurrPiece)
{
	//based ont the piece define what the other edge is for pawns
	int32 XChessBoardEdge = (CurrPiece->PieceColor == EColor::WHITE) ? CurrPiece->ChessBoard->Size - 1 : 0;

	//if the piece is on a tile in the edge
	if (CurrPiece->PlaceAt.X == XChessBoardEdge)
	{
		//and if the piece is a pawn
		APawnPiece* Pawn = Cast<APawnPiece>(CurrPiece);
		if (IsValid(Pawn))
		{
			//it's a pawn promotion
			ChessBoard->MoveStack.Last()->bisPromotion = true;
			Players[CurrentPlayer]->OnPawnPromotion();
			return true;
		}
	}
	return false;
}

void AChess_GameMode::MovePiece(AChessPiece* Piece, ATile* From, ATile* To)
{
	//calculate the new location for the piece
	FVector2D MoveCurrPieceTo = To->GetGridPosition();
	FVector Location = ChessBoard->GetRelativeLocationByXYPosition(MoveCurrPieceTo.X, MoveCurrPieceTo.Y);
	FVector NewLocation = Location + FVector(6, 6, 20);

	//actually move the piece
	Piece->SetActorLocation(NewLocation);

	//the piece in on a new tile
	Piece->PlaceAt = MoveCurrPieceTo;

	//destion and departure tile status are changed
	(Piece->PieceColor == EColor::WHITE) ? To->SetTileStatus(ETileStatus::WHITEPIECE) : To->SetTileStatus(ETileStatus::BLACKPIECE);
	From->SetTileStatus(ETileStatus::EMPTY);
}

AChess_GameMode::AChess_GameMode()
{
	PlayerControllerClass = AChess_PlayerController::StaticClass();
	DefaultPawnClass = AChess_HumanPlayer::StaticClass();
	FieldSize = 8;
}

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
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
			return;
		}

		//set the camera
		float CameraPosX = ((ChessBoard->TileSize * (FieldSize)) / 2) - (ChessBoard->TileSize / 2);
		FVector CameraPos(CameraPosX, CameraPosX, 1500.0f);
		HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());
		
		//add human player in all possible players and in players in game
		AllPlayers.Add(HumanPlayer);
		Players.Add(HumanPlayer);

		//add random player in all possible players
		auto* AIRandom = GetWorld()->SpawnActor<AChess_RandomPlayer>(FVector(), FRotator());
		AllPlayers.Add(AIRandom);

		//add minimax player in all possible players
		auto* AIMinimax = GetWorld()->SpawnActor<AChess_MinimaxPlayer>(FVector(), FRotator());
		AllPlayers.Add(AIMinimax);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cast failed"));
	}
}

void AChess_GameMode::ChoosePlayerAndStartGame(bool difficult)
{
	//CurrentPlayer = FMath::RandRange(0, Players.Num() - 1);
	
	//white player alsways moves fist
	CurrentPlayer = 0;

	//based on what has been clicked on the start, menu set the black player
	if (difficult)
	{ 
		// AI player = 1

		//if it's not the first play, change the second player
		if (Players.Num() == 2)
		{
			Players[1] = AllPlayers[2];
		}
		//if it's the first play, add the second player
		else
		{
			Players.Add(AllPlayers[2]);
		}
	}
	else
	{
		// AI player = 1

		//if it's not the first play, change the second player
		if (Players.Num() == 2)
		{
			Players[1] = AllPlayers[1];
		}
		//if it's the first play, add the second player
		else
		{
			Players.Add(AllPlayers[1]);
		}
	}

	//assign to each player in game, his index in player and his set of pieces
	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
		Players[i]->Set = i == CurrentPlayer ? ESet::WHITE : ESet::BLACK;
	}

	//start the game
	MoveCounter += 1;
	Players[CurrentPlayer]->OnTurn();

}

//get next player using indexes
int32 AChess_GameMode::GetNextPlayer(int32 Player)
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;

}

//give next player the chance to move
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
	EColor OpponentColor = EColor::EMPTY;
	int32 Yposition = Pawn->PlaceAt.Y;
	TArray<ATile*> EnPassantSquare = {};

	//based on the color of the pawn, define its opponent and its direction
	if (Pawn->PieceColor == EColor::WHITE)
	{
		OneStep = 1;
		OneXStep = Pawn->PlaceAt.X + 1;
		OpponentColor = EColor::BLACK;
	}
	else
	{
		OneStep = -1;
		OneXStep = Pawn->PlaceAt.X - 1;
		OpponentColor = EColor::WHITE;
	}

	//get the destination tiles for en passant
	TArray<FVector2D> DiagonalSquareLocations = { FVector2D(OneXStep, Yposition - 1), FVector2D(OneXStep, Yposition + 1) };

	for (FVector2D DiagonalSquareLocation : DiagonalSquareLocations)
	{
		//if the searched position is still in chessboard
		if (ChessBoard->TileMap.Contains(DiagonalSquareLocation))
		{
			//get the tile reference
			ATile* DiagonalSquare = ChessBoard->TileMap[DiagonalSquareLocation];

			//if the tile is empty and the very previous move was an opponent's move
			if (DiagonalSquare->GetTileStatus() == ETileStatus::EMPTY && PreviousPieceMoving->PieceColor == OpponentColor)
			{
				//check if the piece moved is adjacent to the pawn
				if ((PreviousTo->GetGridPosition().X + OneStep) == DiagonalSquare->GetGridPosition().X && PreviousTo->GetGridPosition().Y == DiagonalSquare->GetGridPosition().Y)
				{
					//check if the adjacent piece is a pawn
					APawnPiece* OpponentPawn = Cast<APawnPiece>(PreviousPieceMoving);
					if (IsValid(OpponentPawn))
					{
						//check if it was moved on the very previous turn and it advanced of two squares
						int32 OpponentPawnXStartPosition = PreviousFrom->GetGridPosition().X;
						int32 OpponentPawnXEndPosition = PreviousTo->GetGridPosition().X;
						if (FMath::Abs(OpponentPawnXEndPosition - OpponentPawnXStartPosition) == 2)
						{
							//if conditions are met, add the tile
							EnPassantSquare.Add(DiagonalSquare);
						}
					}
				}
			}
		}
	}
	return EnPassantSquare;
}
