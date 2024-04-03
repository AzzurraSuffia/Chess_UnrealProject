// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_RandomPlayer.h"
#include "GameField.h"
#include "Chess_PlayerController.h"

// Sets default values
AChess_RandomPlayer::AChess_RandomPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*QUI CI VA ISVALID???*/
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called when the game starts or when spawned
void AChess_RandomPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AChess_RandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChess_RandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChess_RandomPlayer::OnPawnPromotion()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Pawn Promotion"));
	GameInstance->SetTurnMessage(TEXT("AI (Random) Pawn Promotion"));
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());

	FTimerHandle TimerHandle;
	int32 RandPieceIdx = -1;

	/*RandPieceIdx assume valori compresi tra 0 e 4-1*/
	RandPieceIdx = FMath::Rand() % 4;

	/*LANCIATA ECCEZIONE SE METTO QUANTO SEGUE NEL CORPO DEL TIMER HANDLE*/
	switch (RandPieceIdx)
	{
	case 0: GameMode->ChessBoard->PromotePawn(EPieceNotation::B); break;
	case 1: GameMode->ChessBoard->PromotePawn(EPieceNotation::N); break;
	case 2: GameMode->ChessBoard->PromotePawn(EPieceNotation::R); break;
	default:
		GameMode->ChessBoard->PromotePawn(EPieceNotation::Q);
	}
}

void AChess_RandomPlayer::OnTurn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Random) Turn"));

	//Modo per settare un Timer per simulare il fatto che l'AI si prenda del tempo per pensare
	FTimerHandle TimerHandle;

	//Lambda function
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
			TArray<AChessPiece*> RandomPlayerPiece = GameMode->ChessBoard->BlackPieceOnChessBoard;
			TArray<ATile*> candidateMoves = {};
			TArray<ATile*> actualMoves = {};
			int32 RandPieceIdx = -1;
			AGameField* ChessBoard = GameMode->ChessBoard;
			UMove* RandomMove = NewObject<UMove>();
			
			if (RandomMove)
			{
				
				do
				{
					RandPieceIdx = FMath::Rand() % RandomPlayerPiece.Num();
					candidateMoves = RandomPlayerPiece[RandPieceIdx]->validMoves();
					for (ATile* candidateTile : candidateMoves)
					{
						if (RandomPlayerPiece[RandPieceIdx]->IsLegal(candidateTile))
						{
							actualMoves.Add(candidateTile);
						}
					}

				} while (actualMoves.IsEmpty());

				int32 RandTileIdx = FMath::Rand() % actualMoves.Num();
				FVector2D MoveCurrPieceTo = actualMoves[RandTileIdx]->GetGridPosition();

				ChessBoard->MoveStack.Add(RandomMove);
				RandomMove->MoveNumber = GameMode->MoveCounter;
				RandomMove->From = ChessBoard->TileMap[RandomPlayerPiece[RandPieceIdx]->PlaceAt];
				RandomMove->To = actualMoves[RandTileIdx];
				RandomMove->PieceMoving = RandomPlayerPiece[RandPieceIdx];

				//ResolveAmbiguitiNotation!!!! (actualMoves[randTileIdx])


				if (actualMoves[RandTileIdx]->GetTileStatus() == ETileStatus::WHITEPIECE)
				{
					/*C'E' UNA CATTURA*/
					for (AChessPiece* whitePiece : ChessBoard->WhitePieceOnChessBoard)
					{
						if (whitePiece->PlaceAt == MoveCurrPieceTo)
						{
							ChessBoard->WhitePieceOnChessBoard.Remove(whitePiece);
							RandomMove->bisCapture = true;
							RandomMove->PieceCaptured = whitePiece;
							//whitePiece->SetActorHiddenInGame(true);
							ChessBoard->MoveOutOfChessBoard(whitePiece, false);
							break;
						}
					}
				}

				ChessBoard->TileMap[RandomPlayerPiece[RandPieceIdx]->PlaceAt]->SetTileStatus(ETileStatus::EMPTY);
				FVector Location = ChessBoard->GetRelativeLocationByXYPosition(MoveCurrPieceTo.X, MoveCurrPieceTo.Y);
				FVector NewLocation = Location + FVector(6, 6, 20);
				RandomPlayerPiece[RandPieceIdx]->SetActorLocation(NewLocation);
				RandomPlayerPiece[RandPieceIdx]->PlaceAt = MoveCurrPieceTo;
				actualMoves[RandTileIdx]->SetTileStatus(ETileStatus::BLACKPIECE);


				if (GameMode->CheckForPawnPromotion(RandomPlayerPiece[RandPieceIdx]))
				{
				}

				bool MoveResult = GameMode->IsGameEnded(RandomMove, ChessBoard->WhiteKing);
				GameMode->OnNewMove.Broadcast();
				if (!MoveResult)
				{
					GameMode->TurnNextPlayer();
				}
				
				AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
				if (IsValid(PlayerController))
				{
					PlayerController->HUDChess->AddMoveWidget(RandomMove);
				}
				/*
				//GameMode->SetCellSign(PlayerNumber, Location);
				*/

			}
			else
			{
				// Pointer to the UMove object is not valid
				UE_LOG(LogTemp, Warning, TEXT("RandomMove pointer not valid."));
			}

			//Il timer è di 1 secondi (aspetta 1 secondi prima di mettere il simbolo)
		}, 1, false);

}

void AChess_RandomPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();

}

void AChess_RandomPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Loses!"));
	//GameInstance->SetTurnMessage(TEXT("AI Loses!"));

}

void AChess_RandomPlayer::OnDraw(EResult DrawOrigin)
{
	if (DrawOrigin == EResult::STALEMATE)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("RANDOM AI:It's a stalemate!"));
		//GameInstance->SetTurnMessage(TEXT("It's a stalemate!"));
	}
}