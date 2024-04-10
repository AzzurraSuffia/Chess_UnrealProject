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

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());

			UMove* BestMove = FindBestMove(GameMode->ChessBoard);
			BestMove->doMove(GameMode);
			GameMode->ChessBoard->MoveStack.Add(BestMove);

			bool MoveResult = GameMode->IsGameEnded(BestMove, GameMode->ChessBoard->WhiteKing);

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
	return BlackPieces.Num() - WhitePieces.Num();
}

int32 AChess_MinimaxPlayer::Utility(int32 Player)
{
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
	if (GameMode->IsKingInCheck(Player))
	{
		//CHECKMATE
		return (Player) ? 10000 : -10000;
	}
	else
	{
		//STALEMATE
		return 0;
	}
}

//posso generalizzare eliminando gli elementi specifici??
int32 AChess_MinimaxPlayer::MiniMax(int32 Depth, bool bisMax, int32 alpha, int32 beta)
{
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
	int32 NextPlayer = (bisMax) ? 0 : 1;

	if (Depth == 0)
	{
		return EvaluateChessboard(GameMode->ChessBoard->WhitePieceOnChessBoard, GameMode->ChessBoard->BlackPieceOnChessBoard);
	}
	else if (!GameMode->PlayerCanMove(NextPlayer))
	{
		return Utility(NextPlayer);
	}

	if (bisMax)
	{
		int32 best = -1000;
		for (AChessPiece* MaxPiece : GameMode->ChessBoard->BlackPieceOnChessBoard)
		{
			TArray<ATile*> MaxCandidateMoves = MaxPiece->validMoves();
			for (ATile* MaxCandidateTile : MaxCandidateMoves)
			{
				if (MaxPiece->IsLegal(MaxCandidateTile))
				{
					/*CODICE QUI DELLA MOSSA*/
					best = FMath::Max(best, MiniMax(Depth - 1, !bisMax, alpha, beta));
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
		int32 best = +1000;
		for (AChessPiece* MinPiece : GameMode->ChessBoard->WhitePieceOnChessBoard)
		{
			TArray<ATile*> MinCandidateMoves = MinPiece->validMoves();
			for (ATile* MinCandidateTile : MinCandidateMoves)
			{
				if (MinPiece->IsLegal(MinCandidateTile))
				{
					/*CODICE QUI DELLA MOSSA*/
					best = FMath::Min(best, MiniMax(Depth - 1, !bisMax, alpha, beta));
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

UMove* AChess_MinimaxPlayer::FindBestMove(AGameField* ChessBoard)
{
	int32 bestVal = -1000;
	int32 alpha = -1000;
	int32 beta = 1000;
	UMove* BestMove = NewObject<UMove>();
	UMove* Move = NewObject<UMove>();
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	if (BestMove && Move)
	{
		for (AChessPiece* MaxPiece : ChessBoard->BlackPieceOnChessBoard)
		{
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
					ATile* to = MaxCandidateTile;
					ATile* from = ChessBoard->TileMap[MaxPiece->PlaceAt];
					TArray<AChessPiece*> OpponentPieceOnBoard = GameMode->ChessBoard->WhitePieceOnChessBoard;
		
					Move->PieceMoving = MaxPiece;
					Move->To = to;
					Move->From = from;

					if (to->GetTileStatus() == OpponentType)
					{
						/*CATTURA DI UN PEZZO*/
						Move->bisCapture = true;

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
							GameMode->ChessBoard->WhitePieceOnChessBoard.Remove(CapturedPiece);
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
							/*PROMOZIONE DEL PEDONE IN REGINA*/
							//il controllo TileMap.Contains mi garantisce che ChangeTileStatus non mi restituisca errore
							AQueenPiece* Queen = ChessBoard->SpawnPieceQueen(EColor::BLACK, InX, InY, (ChessBoard->TileSize / 100));
							//Queen->SetActorHiddenInGame(true);
							
							ChessBoard->BlackPieceOnChessBoard.Add(Queen);
							ChessBoard->BlackPieceOnChessBoard.Remove(MaxPiece);
							Move->bisPromotion = true;
							Move->PiecePromoted = Queen;
						}
					}

					//se il gioco è finito non chiamo ricorsivamente il minimax??

					//---------------------------------------------------------

					int32 moveVal = MiniMax(3, false, alpha, beta);

					/*QUI CI VA L'UNDO??*/
					
					if (moveVal > bestVal)
					{
						/*MANTIENI I CAMPI DELLA STRUTTURA MOSSA SE E' LA MOSSA MIGLIORE*/
						bestVal = moveVal;
						BestMove = Move;
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
					Move->PieceMoving = nullptr;
					Move->To = nullptr;
					Move->From = nullptr;
					Move->bisCapture = false;
					Move->PieceCaptured = nullptr;
					Move->bisPromotion = false;
					Move->PiecePromoted = nullptr;
					//vanno messi a false anche bisCheck e bisCheckmate se li contro sopra
					}
				}
			}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AI (Minimax) bestVal = %d "), bestVal));
		return BestMove;
	}
	return nullptr;
}



