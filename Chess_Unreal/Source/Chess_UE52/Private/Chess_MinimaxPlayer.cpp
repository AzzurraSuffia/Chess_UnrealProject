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

void AChess_MinimaxPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Minimax) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Minimax) Turn"));

	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());

	//disable move buttons and restart button to prevent clicks while AI is thinking
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

	//three seconds delay
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
			AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());

			//find the best move
			UMove* BestMove = FindBestMove(GameMode->ChessBoard, 2);

			//set best move movenumber 
			BestMove->MoveNumber = GameMode->MoveCounter;

			if (BestMove->PieceMoving != nullptr)
			{
				//execute the best move
				BestMove->doMove(GameMode);

				//add it to move stack
				GameMode->ChessBoard->MoveStack.Add(BestMove);

				//check if game ended with this move
				bool MoveResult = GameMode->IsGameEnded(BestMove, GameMode->ChessBoard->WhiteKing);

				//update the current chessboard state with this move
				GameMode->ChessBoard->CurrentChessboardState = BestMove;

				//add the move button of this move to the storyboard
				AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
				if (IsValid(PlayerController))
				{
					UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(BestMove);
					MoveBox->Move = BestMove;
				}

				//if the game isn't ended yet, it's next player's turn
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

int32 AChess_MinimaxPlayer::EvaluateChessboard(TArray<AChessPiece*>& WhitePieces, TArray<AChessPiece*>& BlackPieces)
{
	int BlackScore = 0;
	int WhiteScore = 0;

	//get the weighted sum of all white pieces on chessboard
	for (AChessPiece* WhitePiece : WhitePieces)
	{
		WhiteScore += WhitePiece->PieceWeight();
	}
	//get the weighted sum of all black pieces on chessboard
	for (AChessPiece* BlackPiece : BlackPieces)
	{
		BlackScore += BlackPiece->PieceWeight();
	}

	//the evaluation function is the weighted difference between black and white pieces
	return BlackScore - WhiteScore;
}

int32 AChess_MinimaxPlayer::Utility(int32 Player)
{
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
	if (GameMode->IsKingInCheck(Player))
	{
		//in case of checkmate
		return (Player) ? -1000 : +1000;
	}
	else
	{
		//in case of stalemate (draw)
		return 0;
	}
}

int32 AChess_MinimaxPlayer::AlphaBetaMiniMax(int32 Depth, bool bisMax, int32 alpha, int32 beta, AChessPiece* PreviousPiece, ATile* PreviousTo, ATile* PreviousFrom)
{
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
	int32 CurrentPlayer = (bisMax) ? 1 : 0;
	
	//if a player cannot move, the game ended
	if (!GameMode->PlayerCanMove(CurrentPlayer))
	{
		return Utility(CurrentPlayer);
	}
	//if depth is zero, get a value from evaluation function
	else if (Depth == 0)
	{
		return EvaluateChessboard(GameMode->ChessBoard->WhitePieceOnChessBoard, GameMode->ChessBoard->BlackPieceOnChessBoard);
	} 

	if (bisMax)
	{
		int32 best = -10000;

		//for each black piece on chessboard, analyze every single legal move
		int32 NumBlackPieces = GameMode->ChessBoard->BlackPieceOnChessBoard.Num();
		for (int32 i = 0; i < NumBlackPieces; i++)
		{
			AChessPiece* MaxPiece = GameMode->ChessBoard->BlackPieceOnChessBoard[i];
			TArray<ATile*> MaxCandidateMoves = MaxPiece->validMoves();

			//if it's a pawn, check if en passant is possible
			APawnPiece* CurrPawn = Cast<APawnPiece>(MaxPiece);
			if (IsValid(CurrPawn))
			{
				TArray<ATile*> CandidateEnPassant = GameMode->DetectEnPassant(CurrPawn, PreviousPiece, PreviousTo, PreviousFrom);
				
				//add each en passant destination tile found to the other tiles
				for (ATile* Candidate : CandidateEnPassant)
				{
					MaxCandidateMoves.Add(Candidate);
				}

				//if it's a pawn do not alter the first turn boolean
				if (GameMode->ChessBoard->TileMap[CurrPawn->PlaceAt]->GetGridPosition().X == GameMode->ChessBoard->Size - 2)
				{
					CurrPawn->bfirstMove = true;
				}
			}

			//for each legal destination tile
			for (ATile* MaxCandidateTile : MaxCandidateMoves)
			{
				if (MaxPiece->IsLegal(MaxCandidateTile))
				{
						//execute the move
						ETileStatus MyType = ETileStatus::BLACKPIECE;
						ETileStatus OpponentType = ETileStatus::WHITEPIECE;
						FVector2D MoveCurrPieceTo = MaxCandidateTile->GetGridPosition();
						AChessPiece* CapturedPiece = nullptr;
						int32 CapturedPieceIdx = -1;
						AQueenPiece* Queen = nullptr;
						bool bPromotion = false;
						int32 OneStep = -1;
						TArray<AChessPiece*> OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard;

						ATile* from = GameMode->ChessBoard->TileMap[MaxPiece->PlaceAt];
						ATile* to = MaxCandidateTile;

						//if the destination tile has above an opponent's piece, it's a standard capture
						if (to->GetTileStatus() == OpponentType)
						{
							//get opponent's piece reference
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

							//remove the piece captured from chessboard
							if (CapturedPiece != nullptr)
							{
								GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(CapturedPiece);
							}
						}

						//if the piece analyzed is a pawn, look for en passant
						APawnPiece* Pawn = Cast<APawnPiece>(MaxPiece);
						if (IsValid(Pawn))
						{
							//if a diagonal empty tile is passed, the move has to be managed as an en passant
							if (to->GetTileStatus() == ETileStatus::EMPTY && to->GetGridPosition().Y != from->GetGridPosition().Y)
							{
								//get opponent's pawn position
								FVector2D OpponentPawnPosition = FVector2D(to->GetGridPosition().X - OneStep, to->GetGridPosition().Y);
								
								//get opponent's pawn reference
								int32 Size = OpponentPieceOnBoard.Num();
								for (int32 j = 0; j < Size; j++)
								{
									if (OpponentPieceOnBoard[j]->PlaceAt == OpponentPawnPosition)
									{
										CapturedPiece = OpponentPieceOnBoard[j];
										CapturedPieceIdx = j;
										break;
									}
								}


								if (CapturedPiece != nullptr)
								{
									//remove the piece captured from chessboard
									GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(CapturedPiece);

									//set tile occupied by the opponent's pawn as empty
									GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(ETileStatus::EMPTY);
								}
							}
						}

						//set departure and destionation tiles tile status
						from->SetTileStatus(ETileStatus::EMPTY);
						to->SetTileStatus(MyType);

						//set pawn new position
						MaxPiece->PlaceAt = MoveCurrPieceTo;

						//if the piece analyzed is a pawn, look for pawn promotion
						if(IsValid(Pawn))
						{
							//if the pawn has reached the other edge of chessboard, it's a promotion
							if (Pawn->PlaceAt.X == 0)
							{
								int32 InX = 9;
								int32 InY = -1;
								bPromotion = true;

								//promote the pawn to a queen and set it hidden
								Queen = GameMode->ChessBoard->SpawnPieceQueen(EColor::BLACK, InX, InY, (GameMode->ChessBoard->TileSize / 100));
								Queen->SetActorHiddenInGame(true);

								//move the pawn out of chessboard
								Queen->PlaceAt = Pawn->PlaceAt;
								Pawn->PlaceAt = FVector2D(9, -1);

								//replace the pawn with the new queen
								int32 Index = GameMode->ChessBoard->BlackPieceOnChessBoard.Find(Pawn);
								if (Index != INDEX_NONE)
								{
									GameMode->ChessBoard->BlackPieceOnChessBoard[Index] = Queen;
								}
							}
						}

						//recursive call
						best = FMath::Max(best, AlphaBetaMiniMax(Depth - 1, !bisMax, alpha, beta, MaxPiece, to, from));

						//revert the move
						
						//if the piece analyzed is a pawn, look for en passant
						if (IsValid(Pawn))
						{
							if (CapturedPiece != nullptr)
							{
								//if captured piece position and pawn position are different, it's an en passant
								if (CapturedPiece->PlaceAt.X != MaxPiece->PlaceAt.X)
								{
									to->SetTileStatus(ETileStatus::EMPTY);

									//get opponent's pawn position and restore it
									FVector2D OpponentPawnPosition = FVector2D(to->GetGridPosition().X - OneStep, to->GetGridPosition().Y);
									GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(OpponentType);
									CapturedPiece->PlaceAt = OpponentPawnPosition;
								}
								//if they are the same, it's a standard capture
								else
								{
									to->SetTileStatus(OpponentType);

									//restore captured piece position
									CapturedPiece->PlaceAt = to->GetGridPosition();
								}

								//add the piece captured to the chessboard
								GameMode->ChessBoard->WhitePieceOnChessBoard.Insert(CapturedPiece, CapturedPieceIdx);
								
							}
							else
							{
								to->SetTileStatus(ETileStatus::EMPTY);
							}
						}
						//if the piece analyzed is not a pawn
						else
						{
							//if there was a capture
							if (CapturedPiece != nullptr)
							{
								//restore captured piece position
								to->SetTileStatus(OpponentType);
								CapturedPiece->PlaceAt = to->GetGridPosition();

								//add the piece captured to the chessboard
								GameMode->ChessBoard->WhitePieceOnChessBoard.Insert(CapturedPiece, CapturedPieceIdx);
								
							}
							else
							{
								to->SetTileStatus(ETileStatus::EMPTY);
							}

						}

						from->SetTileStatus(MyType);
						MaxPiece->PlaceAt = from->GetGridPosition();

						//if there was a promotion
						if (bPromotion && Queen != nullptr)
						{
							//move the queen out of chessboard
							Queen->PlaceAt = FVector2D(9, -1);

							//replace the new queen with the pawn
							int32 Index = GameMode->ChessBoard->BlackPieceOnChessBoard.Find(Queen);
							if (Index != INDEX_NONE)
							{
								GameMode->ChessBoard->BlackPieceOnChessBoard[Index] = MaxPiece;
							}

							//destroy the queen
							Queen->Destroy();
						}

						//use alpha-beta pruning to reduce computation
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

		// for each white piece on chessboard, analyze every single legal move
		int32 NumWhitePieces = GameMode->ChessBoard->WhitePieceOnChessBoard.Num();
		for (int32 i = 0; i < NumWhitePieces; i++)
		{
			AChessPiece* MinPiece = GameMode->ChessBoard->WhitePieceOnChessBoard[i];
			TArray<ATile*> MinCandidateMoves = MinPiece->validMoves();
			APawnPiece* CurrPawn = Cast<APawnPiece>(MinPiece);

			//if it's a pawn, check if en passant is possible
			if (IsValid(CurrPawn))
			{
				TArray<ATile*> CandidateEnPassant = GameMode->DetectEnPassant(CurrPawn, PreviousPiece, PreviousTo, PreviousFrom);
				
				//add each en passant destination tile found to the other tiles
				for (ATile* Candidate : CandidateEnPassant)
				{
					MinCandidateMoves.Add(Candidate);
				}

				//if it's a pawn do not alter the first turn boolean
				if (GameMode->ChessBoard->TileMap[CurrPawn->PlaceAt]->GetGridPosition().X == 1)
				{
					CurrPawn->bfirstMove = true;
				}
			}

			//for each legal destination tile
			for (ATile* MinCandidateTile : MinCandidateMoves)
			{
				if (MinPiece->IsLegal(MinCandidateTile))
				{
						//execute the move
						ETileStatus MyType = ETileStatus::WHITEPIECE;
						ETileStatus OpponentType = ETileStatus::BLACKPIECE;
						FVector2D MoveCurrPieceTo = MinCandidateTile->GetGridPosition();
						AChessPiece* CapturedPiece = nullptr;
						int32 CapturedPieceIdx = -1;
						AQueenPiece* Queen = nullptr;
						ATile* to = MinCandidateTile;
						bool bPromotion = false;
						int32 OneStep = 1;
						ATile* from = GameMode->ChessBoard->TileMap[MinPiece->PlaceAt];
						TArray<AChessPiece*> OpponentPieceOnBoard = GameMode->ChessBoard->BlackPieceOnChessBoard;

						//if the destination tile has above an opponent's piece, it's a standard capture
						if (to->GetTileStatus() == OpponentType)
						{
							//get opponent's piece reference
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

							//remove the piece captured from chessboard
							if (CapturedPiece != nullptr)
							{
								GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(CapturedPiece);
							}
						}

						//if the piece analyzed is a pawn, look for en passant
						APawnPiece* Pawn = Cast<APawnPiece>(MinPiece);
						if (IsValid(Pawn))
						{
							//if a diagonal empty tile is passed, the move has to be managed as an en passant
							if (to->GetTileStatus() == ETileStatus::EMPTY && to->GetGridPosition().Y != from->GetGridPosition().Y)
							{
								//get opponent's pawn position
								FVector2D OpponentPawnPosition = FVector2D(to->GetGridPosition().X - OneStep, to->GetGridPosition().Y);
								
								//get opponent's pawn reference
								int32 Size = OpponentPieceOnBoard.Num();
								for (int32 j = 0; j < Size; j++)
								{
									if (OpponentPieceOnBoard[j]->PlaceAt == OpponentPawnPosition)
									{
										CapturedPiece = OpponentPieceOnBoard[j];
										CapturedPieceIdx = j;
										break;
									}
								}
								if (CapturedPiece != nullptr)
								{	
									//remove the piece captured from chessboard
									GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(CapturedPiece);

									//set tile occupied by the opponent's pawn as empty
									GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(ETileStatus::EMPTY);
								}
							}
						}

						//set departure and destionation tiles tile status
						from->SetTileStatus(ETileStatus::EMPTY);
						to->SetTileStatus(MyType);

						//set pawn new position
						MinPiece->PlaceAt = MoveCurrPieceTo;

						//if the piece analyzed is a pawn, look for pawn promotion
						if(IsValid(Pawn))
						{
							//if the pawn has reached the other edge of chessboard, it's a promotion
							if (MinPiece->PlaceAt.X == (GameMode->ChessBoard->Size - 1))
							{
								int32 InX = 9;
								int32 InY = -1;

								//promote the pawn to a queen and set it hidden
								bPromotion = true;
								Queen = GameMode->ChessBoard->SpawnPieceQueen(EColor::WHITE, InX, InY, (GameMode->ChessBoard->TileSize / 100));
								Queen->SetActorHiddenInGame(true);

								//move the pawn out of chessboard
								Queen->PlaceAt = Pawn->PlaceAt;
								Pawn->PlaceAt = FVector2D(9, -1);

								//replace the pawn with the new queen
								int32 Index = GameMode->ChessBoard->WhitePieceOnChessBoard.Find(Pawn);
								if (Index != INDEX_NONE)
								{
									GameMode->ChessBoard->WhitePieceOnChessBoard[Index] = Queen;
								}
							}
						}
						
						//recursive call
						best = FMath::Min(best, AlphaBetaMiniMax(Depth - 1, !bisMax, alpha, beta, MinPiece, to, from));

						//revert the move

						//if the piece analyzed is a pawn, look for en passant
						if (IsValid(Pawn))
						{

							if (CapturedPiece != nullptr)
							{
								//if captured piece position and pawn position are different, it's an en passant
								if (CapturedPiece->PlaceAt.X != MinPiece->PlaceAt.X)
								{
									to->SetTileStatus(ETileStatus::EMPTY);

									//get opponent's pawn position and restore it
									FVector2D OpponentPawnPosition = FVector2D(to->GetGridPosition().X - OneStep, to->GetGridPosition().Y);
									GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(OpponentType);
									CapturedPiece->PlaceAt = OpponentPawnPosition;
								}

								//if they are the same, it's a standard capture
								else
								{
									to->SetTileStatus(OpponentType);

									//restore captured piece position
									CapturedPiece->PlaceAt = to->GetGridPosition();
								}

								//add the piece captured to the chessboard
								GameMode->ChessBoard->BlackPieceOnChessBoard.Insert(CapturedPiece, CapturedPieceIdx);
							}
							else
							{
								to->SetTileStatus(ETileStatus::EMPTY);
							}
						}
						else
						{
							//if there was a capture
							if (CapturedPiece != nullptr)
							{
								//restore captured piece position
								to->SetTileStatus(OpponentType);
								CapturedPiece->PlaceAt = to->GetGridPosition();

								//add the piece captured to the chessboard
								GameMode->ChessBoard->BlackPieceOnChessBoard.Insert(CapturedPiece, CapturedPieceIdx);
							}
							else
							{
								to->SetTileStatus(ETileStatus::EMPTY);
							}
						}

						from->SetTileStatus(MyType);
						MinPiece->PlaceAt = from->GetGridPosition();

						//if there was a promotion
						if (Queen != nullptr && Queen->PlaceAt.X == (GameMode->ChessBoard->Size - 1) && bPromotion)
						{
							//move th queen out of chessboard
							Queen->PlaceAt = FVector2D(-1, 9);

							//replace the new queen with the pawn
							int32 Index = GameMode->ChessBoard->WhitePieceOnChessBoard.Find(Queen);
							if (Index != INDEX_NONE)
							{
								GameMode->ChessBoard->WhitePieceOnChessBoard[Index] = MinPiece;
							}

							//destroy the queen
							Queen->Destroy();
						}

						//use alpha-beta pruning to reduce computation
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
		//for each black piece on chessboard, analyze every single legal move
		int32 NumBlackPieces = ChessBoard->BlackPieceOnChessBoard.Num();
		for (int32 i = 0; i < NumBlackPieces; i++)
		{
			AChessPiece* MaxPiece = ChessBoard->BlackPieceOnChessBoard[i];
			TArray<ATile*> MaxCandidateMoves = MaxPiece->validMoves();
			APawnPiece* CurrPawn = Cast<APawnPiece>(MaxPiece);

			//if the piece to analyze is a pawn, check if en passant is possible
			if (IsValid(CurrPawn))
			{
				UMove* PreviousMove = GameMode->ChessBoard->MoveStack.Last();
				TArray<ATile*> CandidateEnPassant = GameMode->DetectEnPassant(CurrPawn, PreviousMove->PieceMoving, PreviousMove->To, PreviousMove->From);
				
				//add each en passant destination tile found to the other tiles
				for (ATile* Candidate : CandidateEnPassant)
				{
					MaxCandidateMoves.Add(Candidate);
				}

				//if it's a pawn do not alter the first turn boolean
				if (GameMode->ChessBoard->TileMap[CurrPawn->PlaceAt]->GetGridPosition().X == GameMode->ChessBoard->Size - 2)
				{
					CurrPawn->bfirstMove = true;
				}
			}

			//for each legal destination tile
			for (ATile* MaxCandidateTile : MaxCandidateMoves)
			{
				if (MaxPiece->IsLegal(MaxCandidateTile))
				{
					//execute the move
					ETileStatus MyType = ETileStatus::BLACKPIECE;
					ETileStatus OpponentType = ETileStatus::WHITEPIECE;
					FVector2D MoveCurrPieceTo = MaxCandidateTile->GetGridPosition();
					AChessPiece* CapturedPiece = nullptr;
					int32 CapturedPieceIdx = -1;
					AQueenPiece* Queen = nullptr;
					ATile* to = MaxCandidateTile;
					int32 OneStep = -1;
					ATile* from = ChessBoard->TileMap[MaxPiece->PlaceAt];
					bool bPromotion = false;
					TArray<AChessPiece*> OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard;
		
					//fill Move UObject attributes with current move features
					Move->PieceMoving = MaxPiece;
					Move->To = to;
					Move->From = from;

					//if the destination tile has above an opponent's piece, it's a standard capture
					if (to->GetTileStatus() == OpponentType)
					{
						//get opponent's piece reference
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

						//remove the piece captured from chessboard
						if (CapturedPiece != nullptr)
						{
							GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(CapturedPiece);
							Move->PieceCaptured = CapturedPiece;
							Move->bisCapture = true;
						}
					}

					//if the piece analyzed is a pawn, look for en passant
					APawnPiece* Pawn = Cast<APawnPiece>(MaxPiece);
					if (IsValid(Pawn))
					{
						//if a diagonal empty tile is passed, the move has to be managed as an en passant
						if (to->GetTileStatus() == ETileStatus::EMPTY && to->GetGridPosition().Y != from->GetGridPosition().Y)
						{
							//get opponent's pawn position
							FVector2D OpponentPawnPosition = FVector2D(to->GetGridPosition().X - OneStep, to->GetGridPosition().Y);
							
							//get opponent's pawn reference
							int32 Size = OpponentPieceOnBoard.Num();
							for (int32 j = 0; j < Size; j++)
							{
								if (OpponentPieceOnBoard[j]->PlaceAt == OpponentPawnPosition)
								{
									CapturedPiece = OpponentPieceOnBoard[j];
									CapturedPieceIdx = j;
									break;
								}
							}

							//remove the piece captured from chessboard and update move object
							if (CapturedPiece != nullptr)
							{
								GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(CapturedPiece);
								GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(ETileStatus::EMPTY);
								Move->PieceCaptured = CapturedPiece;
								Move->benPassant = true;
							}
						}
					}

					//set departure and destionation tiles tile status
					from->SetTileStatus(ETileStatus::EMPTY);
					to->SetTileStatus(MyType);

					//set pawn new position
					MaxPiece->PlaceAt = MoveCurrPieceTo;

					//if the piece analyzed is a pawn, look for pawn promotion
					if(IsValid(Pawn))
					{
						//if the pawn has reached the other edge of chessboard, it's a promotion
						if (MaxPiece->PlaceAt.X == 0)
						{
							int32 InX = 9;
							int32 InY = -1;
							bPromotion = true;
							
							//promote the pawn to a queen and set it hidden
							Queen = GameMode->ChessBoard->SpawnPieceQueen(EColor::BLACK, InX, InY, (GameMode->ChessBoard->TileSize / 100));
							Queen->SetActorHiddenInGame(true);

							//move the pawn out of chessboard
							Queen->PlaceAt = Pawn->PlaceAt;
							Pawn->PlaceAt = FVector2D(9, -1);

							//replace the pawn with the new queen
							int32 Index = GameMode->ChessBoard->BlackPieceOnChessBoard.Find(Pawn);
							if (Index != INDEX_NONE)
							{
								GameMode->ChessBoard->BlackPieceOnChessBoard[Index] = Queen;
							}

							Move->bisPromotion = true;
							Move->PiecePromoted = Queen;
						}
					}

					//recursive call
					int32 moveVal = AlphaBetaMiniMax(Depth, false, alpha, beta, MaxPiece, to, from);

					//revert the move
					
					//if the piece analyzed is a pawn, look for en passant
					if (IsValid(Pawn))
					{

						if (CapturedPiece != nullptr)
						{
							//if captured piece position and pawn position are different, it's an en passant
							if (CapturedPiece->PlaceAt.X != MaxPiece->PlaceAt.X)
							{
								to->SetTileStatus(ETileStatus::EMPTY);

								//get opponent's pawn position and restore it
								FVector2D OpponentPawnPosition = FVector2D(to->GetGridPosition().X - OneStep, to->GetGridPosition().Y);
								GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(OpponentType);
								CapturedPiece->PlaceAt = OpponentPawnPosition;
								Move->benPassant = true;
							}
							//if they are the same, it's a standard capture
							else
							{
								//restore captured piece position
								to->SetTileStatus(OpponentType);
								CapturedPiece->PlaceAt = to->GetGridPosition();
							}

							//add the piece captured to the chessboard
							GameMode->ChessBoard->WhitePieceOnChessBoard.Insert(CapturedPiece, CapturedPieceIdx);

						}
						else
						{
							to->SetTileStatus(ETileStatus::EMPTY);
						}
					}
					else
					{
						//if there was a capture (not made by a pawn)
						if (CapturedPiece != nullptr)
						{
							//restore captured piece position
							to->SetTileStatus(OpponentType);
							CapturedPiece->PlaceAt = to->GetGridPosition();

							//add the piece captured to the chessboard
							GameMode->ChessBoard->WhitePieceOnChessBoard.Insert(CapturedPiece, CapturedPieceIdx);
							
						}
						else
						{
							to->SetTileStatus(ETileStatus::EMPTY);
						}
					}

					from->SetTileStatus(MyType);
					MaxPiece->PlaceAt = from->GetGridPosition();

					//if there was a promotion
					if (bPromotion && Queen != nullptr)
					{
						//movethe queen out of chessboard
						Queen->PlaceAt = FVector2D(9, -1);

						//replace the new queen with the pawn
						int32 Index = GameMode->ChessBoard->BlackPieceOnChessBoard.Find(Queen);
						if (Index != INDEX_NONE)
						{
							GameMode->ChessBoard->BlackPieceOnChessBoard[Index] = MaxPiece;
						}
					}
					
					
					if (moveVal > bestVal)
					{
						//there's a better move than the last founded, destroy the old move queen if present
						if (BestMove->bisPromotion)
						{
							BestMove->PiecePromoted->Destroy();
						}

						//this move is the new best move
						bestVal = moveVal;
						UMove::CopyMove(BestMove, Move);
					}
					else
					{
						//if it's not a better move, destroy this move queen if present
						if (Move->bisPromotion)
						{
							Move->PiecePromoted->Destroy();
						}
					}

					//clear all move attributes
					UMove::ClearMove(Move);
					Queen = nullptr;
					CapturedPiece = nullptr;
					}
				}
			}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AI (Minimax) bestVal = %d "), bestVal));

		//destroy move 
		Move->ConditionalBeginDestroy();

		return BestMove;
	}
	return nullptr;
}



