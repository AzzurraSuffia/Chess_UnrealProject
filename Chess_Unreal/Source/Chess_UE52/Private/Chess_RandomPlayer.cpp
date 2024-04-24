// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_RandomPlayer.h"
#include "GameField.h"
#include "Chess_PlayerController.h"

// Sets default values
AChess_RandomPlayer::AChess_RandomPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	int32 RandPieceIdx = -1;

	//choose randomly an integer number between 0 and 3
	RandPieceIdx = FMath::Rand() % 4;

	//promote the pawn into a differente piece based on the number extracted 
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

	//one second delay
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
			TArray<AChessPiece*> RandomPlayerPiece = GameMode->ChessBoard->BlackPieceOnChessBoard;
			TArray<ATile*> candidateMoves = {};
			TArray<ATile*> actualMoves = {};
			int32 RandPieceIdx = -1;
			AGameField* ChessBoard = GameMode->ChessBoard;
			UMove* RandomMove = NewObject<UMove>();

			if (RandomMove && IsValid(GameMode))
			{
				do
				{
					//randomly choose a piece from those still on chessboard
					RandPieceIdx = FMath::Rand() % RandomPlayerPiece.Num();

					//find its candidate destination tiles
					candidateMoves = RandomPlayerPiece[RandPieceIdx]->validMoves();

					//if it's a pawn, check if en passant is possible
					APawnPiece* CurrPawn = Cast<APawnPiece>(RandomPlayerPiece[RandPieceIdx]);
					if (IsValid(CurrPawn))
					{
						if (!GameMode->ChessBoard->MoveStack.IsEmpty())
						{
							UMove* PreviousMove = GameMode->ChessBoard->MoveStack.Last();
							TArray<ATile*> CandidateEnPassant = GameMode->DetectEnPassant(CurrPawn, PreviousMove->PieceMoving, PreviousMove->To, PreviousMove->From);
							
							//add each en passant destination tile found to the other tiles
							for (ATile* Candidate : CandidateEnPassant)
							{
								candidateMoves.Add(Candidate);
							}
						}
					}

					//from candidate destination tiles find the legal ones
					for (ATile* candidateTile : candidateMoves)
					{
						if (RandomPlayerPiece[RandPieceIdx]->IsLegal(candidateTile))
						{
							actualMoves.Add(candidateTile);
						}
					}

				//if that piece has no legal moves, repeat
				} while (actualMoves.IsEmpty());

				//randomly choose a destination tile
				int32 RandTileIdx = FMath::Rand() % actualMoves.Num();
				FVector2D MoveCurrPieceTo = actualMoves[RandTileIdx]->GetGridPosition();

				//update Move UObject with the data of this move
				RandomMove->MoveNumber = GameMode->MoveCounter;
				RandomMove->From = ChessBoard->TileMap[RandomPlayerPiece[RandPieceIdx]->PlaceAt];
				RandomMove->To = actualMoves[RandTileIdx];
				RandomMove->PieceMoving = RandomPlayerPiece[RandPieceIdx];

				//if the destination tile has above a white piece, it's a standard capture
				if (actualMoves[RandTileIdx]->GetTileStatus() == ETileStatus::WHITEPIECE)
				{
					//get opponent's piece reference
					for (AChessPiece* whitePiece : ChessBoard->WhitePieceOnChessBoard)
					{
						if (whitePiece->PlaceAt == MoveCurrPieceTo)
						{
							//remove the piece captured from chessboard
							ChessBoard->WhitePieceOnChessBoard.Remove(whitePiece);
							whitePiece->SetActorHiddenInGame(true);
							ChessBoard->MoveOutOfChessBoard(whitePiece);

							//update move fields
							RandomMove->bisCapture = true;
							RandomMove->PieceCaptured = whitePiece;
							break;
						}
					}
				}
				
				ATile* From = ChessBoard->TileMap[RandomPlayerPiece[RandPieceIdx]->PlaceAt];
				ATile* To = actualMoves[RandTileIdx];

				//if the piece randomly selected is a pawn
				APawnPiece* CurrPawn = Cast<APawnPiece>(RandomPlayerPiece[RandPieceIdx]);
				if (IsValid(CurrPawn))
				{
					//if a diagonal empty tile is passed, the move has to be managed as an en passant
					if (To->GetTileStatus() == ETileStatus::EMPTY && To->GetGridPosition().Y != From->GetGridPosition().Y)
					{
						//get opponent's pawn position
						FVector2D OpponentPawnPosition = FVector2D(To->GetGridPosition().X + 1, To->GetGridPosition().Y);

						//get opponent's pawn reference
						for (AChessPiece* whitePiece : ChessBoard->WhitePieceOnChessBoard)
						{
							if (whitePiece->PlaceAt == OpponentPawnPosition)
							{
								//remove the piece captured from chessboard
								ChessBoard->WhitePieceOnChessBoard.Remove(whitePiece);
								whitePiece->SetActorHiddenInGame(true);
								ChessBoard->MoveOutOfChessBoard(whitePiece);

								//update move fields
								RandomMove->benPassant = true;
								RandomMove->PieceCaptured = whitePiece;
								break;
							}
						}

						GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(ETileStatus::EMPTY);
					}
				}

				//move the piece randomly selected
				GameMode->MovePiece(RandomPlayerPiece[RandPieceIdx], From, actualMoves[RandTileIdx]);

				//add the current move to move stack
				ChessBoard->MoveStack.Add(RandomMove);

				//check if a pawn promotion is possible and eventually manage it
				if (GameMode->CheckForPawnPromotion(RandomPlayerPiece[RandPieceIdx]))
				{
				}

				//check if game ended with this move
				bool MoveResult = GameMode->IsGameEnded(RandomMove, ChessBoard->WhiteKing);

				//update the current chessboard state with this move
				GameMode->ChessBoard->CurrentChessboardState = GameMode->ChessBoard->MoveStack.Last();

				//add the move button of this move to the storyboard
				AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
				if (IsValid(PlayerController))
				{
					UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(RandomMove);
					MoveBox->Move = RandomMove;
				}

				//if the game isn't ended yet, it's next player's turn
				if (!MoveResult)
				{
					GameMode->TurnNextPlayer();
				}
			}
			else
			{
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