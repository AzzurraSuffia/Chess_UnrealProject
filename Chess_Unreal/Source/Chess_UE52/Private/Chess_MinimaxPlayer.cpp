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

			if (BestMove->bisCapture)
			{
				/*CODICE QUI*/
			}

			GameMode->MovePiece(BestMove->PieceMoving, BestMove->From, BestMove->To);

			if (BestMove->bisPromotion)
			{
				/*CODICE QUI*/
			}

			GameMode->ChessBoard->MoveStack.Add(BestMove);

			//bool MoveResult = GameMode->IsGameEnded(RandomMove, ChessBoard->WhiteKing);

			AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
			if (IsValid(PlayerController))
			{
				UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(BestMove);
				MoveBox->Move = BestMove;
			}

			//if (!MoveResult)
			//{
				//GameMode->TurnNextPlayer();
			//}

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

//devo controllare e tenere conto delle vittorie ancora prima


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

	if (BestMove)
	{
		for (AChessPiece* MaxPiece : ChessBoard->BlackPieceOnChessBoard)
		{
			TArray<ATile*> MaxCandidateMoves = MaxPiece->validMoves();
			for (ATile* MaxCandidateTile : MaxCandidateMoves)
			{
				if (MaxPiece->IsLegal(MaxCandidateTile))
				{
					/*CODICE QUI DELLA MOSSA*/
					int32 moveVal = MiniMax(3, false, alpha, beta);
					if (moveVal > bestVal)
					{
						/*AGGIORNA I CAMPI DELLA STRUTTURA MOSSA*/
						bestVal = moveVal;
					}
				}
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AI (Minimax) bestVal = %d "), bestVal));
		return BestMove;
	}

	return nullptr;
}



