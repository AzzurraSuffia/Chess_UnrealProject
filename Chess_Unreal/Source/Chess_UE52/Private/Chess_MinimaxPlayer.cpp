// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_MinimaxPlayer.h"
#include "Chess_GameMode.h"

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

			//FVector2D BestMove = FindBestMove(GameMode->GField->TileMap);
			//FVector Location = GameMode->GField->GetRelativeLocationByXYPosition(BestMove.X, BestMove.Y);
			//GameMode->GField->TileMap[BestMove]->SetTileStatus(PlayerNumber, ETileStatus::OCCUPIED);
			//GameMode->SetCellSign(PlayerNumber, Location);

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


int32 AChess_MinimaxPlayer::EvaluateChessboard(TArray<AChessPiece*>& WhitePieces, TArray<AChessPiece*>& BlackPieces, bool bisMax)
{
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
	int32 NextPlayer = (bisMax) ? 0 : 1;

	if (GameMode->PlayerCanMove(NextPlayer))
	{
		/*
		if (GameMode->IsKingInCheck(NextPlayer))
		{

		}
		else
		{

		}
		*/
		return BlackPieces.Num() - WhitePieces.Num();
	}
	else
	{
		//la partita è finita patta o ha vinto l'avversario
		if (GameMode->IsKingInCheck(NextPlayer))
		{
			//CHECKMATE
			return (bisMax) ? 5000 : -5000;
		}
		else
		{
			//STALEMATE
			return 0;
		}
	}
}

int32 AChess_MinimaxPlayer::MinMax(int32 Depth, bool IsMax)
{
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());

	int score = EvaluateChessboard(GameMode->ChessBoard->WhitePieceOnChessBoard, GameMode->ChessBoard->BlackPieceOnChessBoard, true);

	//ha vinto Max
	if (score == 5000)
	{
		return score;
	}
	//ha vinto Min
	if (score == -5000)
	{
		return score;
	}

	if (Depth < 3)
	{
		if (IsMax)
		{
			int32 best = -1000;
			/*LOGICA QUI*/
			/*devo muovere ogni pedina in ogni possibile cella, */
			for (AChessPiece* BlackPiece : GameMode->ChessBoard->BlackPieceOnChessBoard)
			{
				TArray<ATile*> candidateMoves = BlackPiece->validMoves();
				for (ATile* candidateTile : candidateMoves)
				{
					if (BlackPiece->IsLegal(candidateTile))
					{
						//sposta la pedina e computa il nuovo giro

					}
				}
			}
			return best;
		}
		else
		{
			int32 best = 1000;
			/*LOGICA QUI*/
			for (AChessPiece* WhitePiece : GameMode->ChessBoard->WhitePieceOnChessBoard)
			{
				TArray<ATile*> candidateMoves = WhitePiece->validMoves();
				for (ATile* candidateTile : candidateMoves)
				{
					if (WhitePiece->IsLegal(candidateTile))
					{

					}
				}
			}
			return best;
		}
	}
	else
	{
		return score;
	}
}



