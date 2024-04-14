// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_MinimaxPlayer.h"
#include "Chess_GameMode.h"
#include "Chess_PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AChess_MinimaxPlayer::AChess_MinimaxPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

// Called when the game starts or when spawned
void AChess_MinimaxPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChess_MinimaxPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChess_MinimaxPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChess_MinimaxPlayer::OnPawnPromotion()
{

}

void AChess_MinimaxPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Minimax) Turn"));

	AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PlayerController))
	{
		for (UUI_MoveBox* MoveBox : PlayerController->HUDChess->AllMoves)
		{
			MoveBox->SetIsEnabled(false);
		}
		PlayerController->HUDChess->ResetButtonWidget->SetIsEnabled(false);
	}

	FTimerHandle TimerHandle;

	//Lambda function
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
			AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());

			UMove* BestMove = FindBestMove(GameMode->ChessBoard, 2);
			BestMove->MoveNumber = GameMode->MoveCounter;
			if (BestMove->PieceMoving == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GENERATA MOSSA VUOTA"));
			}
			else
			{
				BestMove->doMove(GameMode);
				GameMode->ChessBoard->MoveStack.Add(BestMove);

				bool MoveResult = GameMode->IsGameEnded(BestMove, GameMode->ChessBoard->WhiteKing);

				GameMode->ChessBoard->CurrentChessboardState = BestMove;

				AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
				if (IsValid(PlayerController))
				{
					UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(BestMove);
					MoveBox->Move = BestMove;
				}

				if (!MoveResult)
				{
					GameMode->TurnNextPlayer();
				}
			}
	}, 3, false);
}

void AChess_MinimaxPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

void AChess_MinimaxPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Loses!"));
	// GameInstance->SetTurnMessage(TEXT("AI Loses!"));
}

void AChess_MinimaxPlayer::OnDraw(EResult DrawOrigin)
{
	if (DrawOrigin == EResult::STALEMATE)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("AI (Minimax):It's a stalemate!"));
		//GameInstance->SetTurnMessage(TEXT("It's a stalemate!"));
	}
}

void AChess_MinimaxPlayer::DoMinimaxMove(AChessPiece* Piece, ATile* CandidateTile)
{
	/*
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
	ETileStatus MyType = ETileStatus::BLACKPIECE;
	ETileStatus OpponentType = ETileStatus::WHITEPIECE;
	FVector2D MoveCurrPieceTo = CandidateTile->GetGridPosition();
	AChessPiece* CapturedPiece = nullptr;
	int32 CapturedPieceIdx = -1;
	AQueenPiece* Queen = nullptr;
	ATile* to = CandidateTile;
	bool bPromotion = false;
	ATile* from = GameMode->ChessBoard->TileMap[Piece->PlaceAt];
	TArray<AChessPiece*> OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard;

	if (to->GetTileStatus() == OpponentType)
	{
		int32 Size = OpponentPieceOnBoard.Num();
		for (int32 j = 0; j < Size; j++)
		{
			if (OpponentPieceOnBoard[j]->PlaceAt == MoveCurrPieceTo)
			{
				CapturedPiece = OpponentPieceOnBoard[j];
				CapturedPieceIdx = j;
				break;
			}
		}
		if (CapturedPiece != nullptr)
		{
			GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(CapturedPiece);
			CapturedPiece->PlaceAt = FVector2D(-1, -1);
		}
	}

	from->SetTileStatus(ETileStatus::EMPTY);
	to->SetTileStatus(MyType);
	Piece->PlaceAt = MoveCurrPieceTo;

	if (Piece->PlaceAt.X == 0)
	{
		APawnPiece* Pawn = Cast<APawnPiece>(Piece);
		if (IsValid(Pawn))
		{
			int32 InX = 9;
			int32 InY = -1;
			bPromotion = true;
			//il controllo TileMap.Contains mi garantisce che ChangeTileStatus non mi restituisca errore
			Queen = GameMode->ChessBoard->SpawnPieceQueen(EColor::BLACK, InX, InY, (GameMode->ChessBoard->TileSize / 100));
			//Queen->SetActorHiddenInGame(true);
			Queen->PlaceAt = Pawn->PlaceAt;
			Pawn->PlaceAt = FVector2D(9, -1);

			int32 Index = GameMode->ChessBoard->BlackPieceOnChessBoard.Find(Pawn);
			if (Index != INDEX_NONE)
			{
				GameMode->ChessBoard->BlackPieceOnChessBoard[Index] = Queen;
			}
		}
	}*/
}


int32 AChess_MinimaxPlayer::EvaluateChessboard(TArray<AChessPiece*>& WhitePieces, TArray<AChessPiece*>& BlackPieces)
{
	int BlackScore = 0;
	int WhiteScore = 0;

	for (AChessPiece* WhitePiece : WhitePieces)
	{
		WhiteScore += WhitePiece->PieceWeight();
	}
	for (AChessPiece* BlackPiece : BlackPieces)
	{
		BlackScore += BlackPiece->PieceWeight();
	}

	return BlackScore - WhiteScore;
}

int32 AChess_MinimaxPlayer::Utility(int32 Player)
{
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
	if (GameMode->IsKingInCheck(Player))
	{
		//CHECKMATE
		return (Player) ? -1000 : +1000;
	}
	else
	{
		//STALEMATE
		return 0;
	}
}

//posso generalizzare eliminando gli elementi specifici??
int32 AChess_MinimaxPlayer::AlphaBetaMiniMax(int32 Depth, bool bisMax, int32 alpha, int32 beta)
{
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
	int32 CurrentPlayer = (bisMax) ? 1 : 0;
	
	if (!GameMode->PlayerCanMove(CurrentPlayer))
	{
		return Utility(CurrentPlayer);
	}
	else if (Depth == 0)
	{
		return EvaluateChessboard(GameMode->ChessBoard->WhitePieceOnChessBoard, GameMode->ChessBoard->BlackPieceOnChessBoard);
	} 

	if (bisMax)
	{
		int32 best = -10000;
		int32 NumBlackPieces = GameMode->ChessBoard->BlackPieceOnChessBoard.Num();
		for (int32 i = 0; i < NumBlackPieces; i++)
		{
			AChessPiece* MaxPiece = GameMode->ChessBoard->BlackPieceOnChessBoard[i];
			TArray<ATile*> MaxCandidateMoves = MaxPiece->validMoves();
			for (ATile* MaxCandidateTile : MaxCandidateMoves)
			{
				if (MaxPiece->IsLegal(MaxCandidateTile))
				{
					/*CODICE QUI DELLA MOSSA*/
					// ------------------------------------------------------------
						ETileStatus MyType = ETileStatus::BLACKPIECE;
						ETileStatus OpponentType = ETileStatus::WHITEPIECE;
						FVector2D MoveCurrPieceTo = MaxCandidateTile->GetGridPosition();
						AChessPiece* CapturedPiece = nullptr;
						int32 CapturedPieceIdx = -1;
						AQueenPiece* Queen = nullptr;
						bool bPromotion = false;
						TArray<AChessPiece*> OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard;

						ATile* from = GameMode->ChessBoard->TileMap[MaxPiece->PlaceAt];
						ATile* to = MaxCandidateTile;

						if (to->GetTileStatus() == ETileStatus::WHITEPIECE)
						{
							/*CATTURA DI UN PEZZO*/
							int32 Size = OpponentPieceOnBoard.Num();
							for (int32 j = 0; j < Size; j++)
							{
								if (OpponentPieceOnBoard[j]->PlaceAt == MoveCurrPieceTo)
								{
									CapturedPiece = OpponentPieceOnBoard[j];
									CapturedPieceIdx = j;
									break;
								}
							}
							if (CapturedPiece != nullptr)
							{
								GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(CapturedPiece);
								CapturedPiece->PlaceAt = FVector2D(-1, -1);
							}
						}

						from->SetTileStatus(ETileStatus::EMPTY);
						to->SetTileStatus(MyType);
						MaxPiece->PlaceAt = MoveCurrPieceTo;

						/*PROMOZIONE*/
						if (MaxPiece->PlaceAt.X == 0)
						{
							APawnPiece* Pawn = Cast<APawnPiece>(MaxPiece);
							if (IsValid(Pawn))
							{
								int32 InX = 9;
								int32 InY = -1;
								bPromotion = true;
								//il controllo TileMap.Contains mi garantisce che ChangeTileStatus non mi restituisca errore
								Queen = GameMode->ChessBoard->SpawnPieceQueen(EColor::BLACK, InX, InY, (GameMode->ChessBoard->TileSize / 100));
								//Queen->SetActorHiddenInGame(true);

								/*DEVO AGGIORNARE ANCHE QUESTO PERCHE' NELLA CATTURA VIENE CONTROLLATO PER TROVARE I PEZZI*/
								Queen->PlaceAt = Pawn->PlaceAt;
								Pawn->PlaceAt = FVector2D(9, -1);

								int32 Index = GameMode->ChessBoard->BlackPieceOnChessBoard.Find(Pawn);
								if (Index != INDEX_NONE)
								{
									GameMode->ChessBoard->BlackPieceOnChessBoard[Index] = Queen;
								}
							}
						}

						//se il gioco è finito non chiamo ricorsivamente il minimax??
						//-------------------------------------------------------------
						best = FMath::Max(best, AlphaBetaMiniMax(Depth - 1, !bisMax, alpha, beta));

						/*CODICE QUI DELL'UNDO DELLA MOSSA*/
						// ------------------------------------------------------------
						from->SetTileStatus(MyType);
						MaxPiece->PlaceAt = from->GetGridPosition();

						if (CapturedPiece != nullptr)
						{
							/*DEVO RIPRISTINARE IL PEZZO CATTURATO*/
							to->SetTileStatus(OpponentType);
							GameMode->ChessBoard->WhitePieceOnChessBoard.Insert(CapturedPiece, CapturedPieceIdx);
							CapturedPiece->PlaceAt = to->GetGridPosition();
						}
						else
						{
							to->SetTileStatus(ETileStatus::EMPTY);
						}

						APawnPiece* Pawn = Cast<APawnPiece>(MaxPiece);
						if (IsValid(Pawn))
						{
							int32 PawnStartPosition = (GameMode->ChessBoard->Size - 2);
							if (from->GetGridPosition().X == PawnStartPosition)
							{
								Pawn->bfirstMove = true;
							}
						}

						if (bPromotion && Queen != nullptr)
						{
							Queen->PlaceAt = FVector2D(9, -1);

							int32 Index = GameMode->ChessBoard->BlackPieceOnChessBoard.Find(Queen);
							if (Index != INDEX_NONE)
							{
								GameMode->ChessBoard->BlackPieceOnChessBoard[Index] = MaxPiece;
							}
							Queen->Destroy();
						}
						//-------------------------------------------------------------
						if (best >= beta)
						{
							return best;
						}
						alpha = FMath::Max(alpha, best);
				}
			}
		}
		return best;
	}
	else
	{
		int32 best = +10000;
		int32 NumWhitePieces = GameMode->ChessBoard->WhitePieceOnChessBoard.Num();
		for (int32 i = 0; i < NumWhitePieces; i++)
		{
			AChessPiece* MinPiece = GameMode->ChessBoard->WhitePieceOnChessBoard[i];
			TArray<ATile*> MinCandidateMoves = MinPiece->validMoves();
			for (ATile* MinCandidateTile : MinCandidateMoves)
			{
				if (MinPiece->IsLegal(MinCandidateTile))
				{
					/*CODICE QUI DELLA MOSSA*/
					// ------------------------------------------------------------
						ETileStatus MyType = ETileStatus::WHITEPIECE;
						ETileStatus OpponentType = ETileStatus::BLACKPIECE;
						FVector2D MoveCurrPieceTo = MinCandidateTile->GetGridPosition();
						AChessPiece* CapturedPiece = nullptr;
						int32 CapturedPieceIdx = -1;
						AQueenPiece* Queen = nullptr;
						ATile* to = MinCandidateTile;
						bool bPromotion = false;
						ATile* from = GameMode->ChessBoard->TileMap[MinPiece->PlaceAt];
						TArray<AChessPiece*> OpponentPieceOnBoard = GameMode->ChessBoard->BlackPieceOnChessBoard;

						if (to->GetTileStatus() == OpponentType)
						{
							/*CATTURA DI UN PEZZO*/
							int32 Size = OpponentPieceOnBoard.Num();
							for (int32 j = 0; j < Size; j++)
							{
								if (OpponentPieceOnBoard[j]->PlaceAt == MoveCurrPieceTo)
								{
									CapturedPiece = OpponentPieceOnBoard[j];
									CapturedPieceIdx = j;
									break;
								}
							}
							if (CapturedPiece != nullptr)
							{
								GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(CapturedPiece);
								CapturedPiece->PlaceAt = FVector2D(9, 9);
							}
						}

						from->SetTileStatus(ETileStatus::EMPTY);
						to->SetTileStatus(MyType);
						MinPiece->PlaceAt = MoveCurrPieceTo;

						/*PROMOZIONE*/
						if (MinPiece->PlaceAt.X == (GameMode->ChessBoard->Size - 1))
						{
							APawnPiece* Pawn = Cast<APawnPiece>(MinPiece);
							if (IsValid(Pawn))
							{
								int32 InX = 9;
								int32 InY = -1;
								bPromotion = true;
								//il controllo TileMap.Contains mi garantisce che ChangeTileStatus non mi restituisca errore
								Queen = GameMode->ChessBoard->SpawnPieceQueen(EColor::WHITE, InX, InY, (GameMode->ChessBoard->TileSize / 100));
								//Queen->SetActorHiddenInGame(true);

								/*DEVO AGGIORNARE ANCHE QUESTO PERCHE' NELLA CATTURA VIENE CONTROLLATO PER TROVARE I PEZZI*/
								Queen->PlaceAt = Pawn->PlaceAt;
								Pawn->PlaceAt = FVector2D(9, -1);

								int32 Index = GameMode->ChessBoard->WhitePieceOnChessBoard.Find(Pawn);
								if (Index != INDEX_NONE)
								{
									GameMode->ChessBoard->WhitePieceOnChessBoard[Index] = Queen;
								}
							}
						}

						//se il gioco è finito non chiamo ricorsivamente il minimax??
						//-------------------------------------------------------------
						best = FMath::Min(best, AlphaBetaMiniMax(Depth - 1, !bisMax, alpha, beta));

						/*CODICE QUI DELL'UNDO DELLA MOSSA*/
						// ------------------------------------------------------------
						from->SetTileStatus(MyType);
						MinPiece->PlaceAt = from->GetGridPosition();
						CapturedPiece = CapturedPiece;

						if (CapturedPiece != nullptr)
						{
							/*DEVO RIPRISTINARE IL PEZZO CATTURATO*/
							to->SetTileStatus(OpponentType);
							GameMode->ChessBoard->BlackPieceOnChessBoard.Insert(CapturedPiece, CapturedPieceIdx);
							CapturedPiece->PlaceAt = to->GetGridPosition();
						}
						else
						{
							to->SetTileStatus(ETileStatus::EMPTY);
						}

						APawnPiece* Pawn = Cast<APawnPiece>(MinPiece);
						if (IsValid(Pawn))
						{
							int32 PawnStartPosition = 1;
							if (from->GetGridPosition().X == PawnStartPosition)
							{
								Pawn->bfirstMove = true;
							}
						}

						if (Queen != nullptr && Queen->PlaceAt.X == (GameMode->ChessBoard->Size - 1) && bPromotion)
						{
							Queen->PlaceAt = FVector2D(-1, 9);

							int32 Index = GameMode->ChessBoard->WhitePieceOnChessBoard.Find(Queen);
							if (Index != INDEX_NONE)
							{
								GameMode->ChessBoard->WhitePieceOnChessBoard[Index] = MinPiece;
							}
							Queen->Destroy();
						}
						//-------------------------------------------------------------
						if (best <= alpha)
						{
							return best;
						}
						beta = FMath::Min(beta, best);
				}
			}
		}
		return best;
	}
}

UMove* AChess_MinimaxPlayer::FindBestMove(AGameField* ChessBoard, int32 Depth)
{
	int32 bestVal = -10000;
	int32 alpha = -10000;
	int32 beta = 10000;
	UMove* BestMove = NewObject<UMove>();
	UMove* Move = NewObject<UMove>();
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	if (BestMove && Move)
	{
		int32 NumBlackPieces = ChessBoard->BlackPieceOnChessBoard.Num();
		for (int32 i = 0; i < NumBlackPieces; i++)
		{
			AChessPiece* MaxPiece = ChessBoard->BlackPieceOnChessBoard[i];
			TArray<ATile*> MaxCandidateMoves = MaxPiece->validMoves();
			for (ATile* MaxCandidateTile : MaxCandidateMoves)
			{
				if (MaxPiece->IsLegal(MaxCandidateTile))
				{
					/*CODICE QUI DELLA MOSSA*/
					//---------------------------------------------------------
					ETileStatus MyType = ETileStatus::BLACKPIECE;
					ETileStatus OpponentType = ETileStatus::WHITEPIECE;
					FVector2D MoveCurrPieceTo = MaxCandidateTile->GetGridPosition();
					AChessPiece* CapturedPiece = nullptr;
					int32 CapturedPieceIdx = -1;
					AQueenPiece* Queen = nullptr;
					ATile* to = MaxCandidateTile;
					ATile* from = ChessBoard->TileMap[MaxPiece->PlaceAt];
					bool bPromotion = false;
					TArray<AChessPiece*> OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard;
		
					Move->PieceMoving = MaxPiece;
					Move->To = to;
					Move->From = from;

					if (to->GetTileStatus() == OpponentType)
					{
						/*CATTURA DI UN PEZZO*/
						Move->bisCapture = true;

						int32 Size = OpponentPieceOnBoard.Num();
						for (int32 j = 0; j < Size; j++)
						{
							if (OpponentPieceOnBoard[j]->PlaceAt == MoveCurrPieceTo)
							{
								CapturedPiece = OpponentPieceOnBoard[j];
								CapturedPieceIdx = j;
								break;
							}
						}
						if (CapturedPiece != nullptr)
						{
							GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(CapturedPiece);
							CapturedPiece->PlaceAt = FVector2D(-1, -1);
							Move->PieceCaptured = CapturedPiece;
						}
					}
					
					from->SetTileStatus(ETileStatus::EMPTY);
					to->SetTileStatus(MyType);
					MaxPiece->PlaceAt = MoveCurrPieceTo;

					/*PROMOZIONE*/
					if (MaxPiece->PlaceAt.X == 0)
					{
						APawnPiece* Pawn = Cast<APawnPiece>(MaxPiece);
						if (IsValid(Pawn))
						{
							int32 InX = 9;
							int32 InY = -1;
							bPromotion = true;
							//il controllo TileMap.Contains mi garantisce che ChangeTileStatus non mi restituisca errore
							Queen = ChessBoard->SpawnPieceQueen(EColor::BLACK, InX, InY, (ChessBoard->TileSize / 100));
							//Queen->SetActorHiddenInGame(true);
							
							/*DEVO AGGIORNARE ANCHE QUESTO PERCHE' NELLA CATTURA VIENE CONTROLLATO PER TROVARE I PEZZI*/
							Queen->PlaceAt = Pawn->PlaceAt;
							Pawn->PlaceAt = FVector2D(9, -1);

							int32 Index = GameMode->ChessBoard->BlackPieceOnChessBoard.Find(Pawn);
							if (Index != INDEX_NONE)
							{
								GameMode->ChessBoard->BlackPieceOnChessBoard[Index] = Queen;
							}

							Move->bisPromotion = true;
							Move->PiecePromoted = Queen;
						}
					}

					//se il gioco è finito non chiamo ricorsivamente il minimax??

					//---------------------------------------------------------

					int32 moveVal = AlphaBetaMiniMax(Depth, false, alpha, beta);

					/*QUI CI VA L'UNDO*/
					//---------------------------------------------------------
					from->SetTileStatus(MyType);
					MaxPiece->PlaceAt = from->GetGridPosition();

					if (CapturedPiece != nullptr)
					{
						/*DEVO RIPRISTINARE IL PEZZO CATTURATO*/
						to->SetTileStatus(OpponentType);
						CapturedPiece->PlaceAt = to->GetGridPosition();
						GameMode->ChessBoard->WhitePieceOnChessBoard.Insert(CapturedPiece, CapturedPieceIdx);
					}
					else
					{
						to->SetTileStatus(ETileStatus::EMPTY);
					}

					if (Queen != nullptr && bPromotion)
					{
						Queen->PlaceAt = FVector2D(9, -1);
						int32 Index = ChessBoard->BlackPieceOnChessBoard.Find(Queen);
						if (Index != INDEX_NONE)
						{
							ChessBoard->BlackPieceOnChessBoard[Index] = MaxPiece;
						}
					}

					//---------------------------------------------------------
					
					if (moveVal > bestVal)
					{
						/*C'E' UNA MOSSA MIGLIORE DELL'ULTIMA TROVATA: DISTRUGGI LA VECCHIA REGINA*/
						if (BestMove->bisPromotion)
						{
							BestMove->PiecePromoted->Destroy();
						}

						/*ASSEGNA A BESTMOVE LA NUOVA MOSSA MIGLIORE*/
						bestVal = moveVal;
						UMove::CopyMove(BestMove, Move);
					}
					else
					{
						/*DISTRUGGI LA REGINA CREATA SE C'E' STATA UNA PROMOZIONE*/
						if (Move->bisPromotion)
						{
							Move->PiecePromoted->Destroy();
						}
					}

					/*RESETTA I CAMPI DELLA STRUTTURA MOSSA*/
					UMove::ClearMove(Move);
					Queen = nullptr;
					CapturedPiece = nullptr;
					}
				}
			}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AI (Minimax) bestVal = %d "), bestVal));
		Move->ConditionalBeginDestroy();
		return BestMove;
	}
	return nullptr;
}



