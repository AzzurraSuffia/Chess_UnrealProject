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
	//GameInstance->SetTurnMessage(TEXT("AI (Random) Pawn Promotion"));

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

	AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PlayerController))
	{
		for (UUI_MoveBox* MoveBox : PlayerController->HUDChess->AllMoves)
		{
			MoveBox->SetIsEnabled(false);
		}
		PlayerController->HUDChess->ResetButtonWidget->SetIsEnabled(false);
	}

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
			
			if (!GameMode->ChessBoard->MoveStack.IsEmpty())
			{
				TArray<ATile*> PreviousTiles = { GameMode->ChessBoard->CurrentChessboardState->To,GameMode->ChessBoard->CurrentChessboardState->From };
				GameMode->ChessBoard->RestoreSquaresColor(PreviousTiles);
			}

			if (RandomMove && IsValid(GameMode))
			{
				do
				{
					RandPieceIdx = FMath::Rand() % RandomPlayerPiece.Num();
					candidateMoves = RandomPlayerPiece[RandPieceIdx]->validMoves();
					APawnPiece* CurrPawn = Cast<APawnPiece>(RandomPlayerPiece[RandPieceIdx]);
					if (IsValid(CurrPawn))
					{
						if (!GameMode->ChessBoard->MoveStack.IsEmpty())
						{
							UMove* PreviousMove = GameMode->ChessBoard->MoveStack.Last();
							TArray<ATile*> CandidateEnPassant = GameMode->DetectEnPassant(CurrPawn, PreviousMove->PieceMoving, PreviousMove->To, PreviousMove->From);
							for (ATile* Candidate : CandidateEnPassant)
							{
								candidateMoves.Add(Candidate);
							}
						}
					}
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

				RandomMove->MoveNumber = GameMode->MoveCounter;
				RandomMove->From = ChessBoard->TileMap[RandomPlayerPiece[RandPieceIdx]->PlaceAt];
				RandomMove->To = actualMoves[RandTileIdx];
				RandomMove->PieceMoving = RandomPlayerPiece[RandPieceIdx];

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
							whitePiece->SetActorHiddenInGame(true);
							ChessBoard->MoveOutOfChessBoard(whitePiece);
							break;
						}
					}
				}
				
				ATile* From = ChessBoard->TileMap[RandomPlayerPiece[RandPieceIdx]->PlaceAt];
				ATile* To = actualMoves[RandTileIdx];

				/*EN PASSANT*/
				APawnPiece* CurrPawn = Cast<APawnPiece>(RandomPlayerPiece[RandPieceIdx]);
				if (IsValid(CurrPawn))
				{
					if (To->GetTileStatus() == ETileStatus::EMPTY && To->GetGridPosition().Y != From->GetGridPosition().Y)
					{
						FVector2D OpponentPawnPosition = FVector2D(To->GetGridPosition().X + 1, To->GetGridPosition().Y);
						for (AChessPiece* whitePiece : ChessBoard->WhitePieceOnChessBoard)
						{
							if (whitePiece->PlaceAt == OpponentPawnPosition)
							{
								ChessBoard->WhitePieceOnChessBoard.Remove(whitePiece);
								RandomMove->benPassant = true;
								RandomMove->PieceCaptured = whitePiece;
								whitePiece->SetActorHiddenInGame(true);
								ChessBoard->MoveOutOfChessBoard(whitePiece);
								break;
							}
						}

						GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(ETileStatus::EMPTY);
					}
				}

				GameMode->MovePiece(RandomPlayerPiece[RandPieceIdx], From, actualMoves[RandTileIdx]);

				ChessBoard->MoveStack.Add(RandomMove);

				if (GameMode->CheckForPawnPromotion(RandomPlayerPiece[RandPieceIdx]))
				{
				}

				bool MoveResult = GameMode->IsGameEnded(RandomMove, ChessBoard->WhiteKing);

				GameMode->ChessBoard->CurrentChessboardState = GameMode->ChessBoard->MoveStack.Last();

				AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
				if (IsValid(PlayerController))
				{
					UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(RandomMove);
					MoveBox->Move = RandomMove;
				}

				if (!MoveResult)
				{
					GameMode->TurnNextPlayer();
				}
			}
			else
			{
				// Pointer to the UMove object is not valid
				UE_LOG(LogTemp, Warning, TEXT("RandomMove or GameMode pointer not valid."));
			}
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