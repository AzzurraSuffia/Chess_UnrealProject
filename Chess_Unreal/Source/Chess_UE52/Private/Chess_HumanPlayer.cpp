// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_HumanPlayer.h"
#include "GameField.h"
#include "Chess_GameMode.h"
#include "Move.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Chess_PlayerController.h"

// Sets default values
AChess_HumanPlayer::AChess_HumanPlayer()
{
	//Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//create a camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	//set the camera as RootComponent
	SetRootComponent(Camera);

	//get the game instance reference
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//default init values
	PlayerNumber = -1;

	Set = ESet::EMPTY;
}

/*
* Function that manages replay: update move data structures and widgets to resume the game from the move clicked
*/
bool AChess_HumanPlayer::ManageReplay(UMove* FirstReplayMove)
{
	//move clicked in the storyboard was a black move: the human move is valid (confirm the move)
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode->ChessBoard->CurrentChessboardState->PieceMoving->PieceColor == EColor::BLACK)
	{
		int32 MoveDoneIdx = GameMode->ChessBoard->MoveStack.Find(GameMode->ChessBoard->MoveStack.Last());
		int32 ClickedMoveIdx = GameMode->ChessBoard->MoveStack.Find(GameMode->ChessBoard->CurrentChessboardState);

		if (ClickedMoveIdx != INDEX_NONE && MoveDoneIdx != INDEX_NONE)
		{
			AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
			if (IsValid(PlayerController))
			{
				GameMode->MoveCounter = GameMode->ChessBoard->MoveStack[ClickedMoveIdx]->MoveNumber + 1;
				GameMode->ChessBoard->MoveStack.Last()->MoveNumber = GameMode->MoveCounter;

				int32 LastTextBoxToPreserve = ClickedMoveIdx/2;
				int32 LastTextBoxIdx = PlayerController->HUDChess->OtherNotationComponents.Num() - 1;

				for (int32 i = LastTextBoxIdx; i > LastTextBoxToPreserve; i--)
				{
					PlayerController->HUDChess->OtherNotationComponents[i]->ConditionalBeginDestroy();
				}
				PlayerController->HUDChess->OtherNotationComponents.SetNum(LastTextBoxToPreserve+1);

				for (int32 i = MoveDoneIdx - 1; i > ClickedMoveIdx; i--)
				{
					UMove* Move = GameMode->ChessBoard->MoveStack[i];

					//non devo fare l'undo della mossa perchè è già stato fatto con il click nello storico

					//tornando indietro devo ripristinare i pezzi catturati in quel range di mosse (NON devo distruggere i pezzi catturati)
					//tornando indietro devo ripristinare i pedoni promossi (DEVO distruggere i pezzi in cui sono stati promossi)
					if (Move->bisPromotion)
					{
						Move->PiecePromoted->Destroy();
					}
					Move->ConditionalBeginDestroy();
					GameMode->ChessBoard->MoveStack.Remove(Move);

					PlayerController->HUDChess->AllMoves[i]->ConditionalBeginDestroy();
				}
				PlayerController->HUDChess->AllMoves.SetNum(ClickedMoveIdx+1);
			}
		}
		return true;
	}
	else
	{
		//move clicked in the storyboard was a white move: the human move is not valid (undo the move)
		FirstReplayMove->UndoMove(GameMode);
		GameMode->ChessBoard->MoveStack.Remove(FirstReplayMove);
		FirstReplayMove->ConditionalBeginDestroy();

		//enable storyboard bottons to click on another move
		AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
		if (IsValid(PlayerController))
		{
			for (UUI_MoveBox* MoveBox : PlayerController->HUDChess->AllMoves)
			{
				MoveBox->SetIsEnabled(true);
			}
		}
		GameMode->ChessBoard->CurrentChessboardState->To->SetTileColor(5);
		GameMode->ChessBoard->CurrentChessboardState->From->SetTileColor(5);

		return false;
	}
}

// Called when the game starts or when spawned
void AChess_HumanPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChess_HumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChess_HumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/*
* When bpromotionFlag is false, OnPawnPromotion() set visibile the pieces menu to the user (human pawn promotion start)
* When bpromotionFlag is true, OnPawnPromotion() checks if game ended and add the move widgetto the storyboard (human pawn promotion end)
*/
void AChess_HumanPlayer::OnPawnPromotion()
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode->bpromotionFlag)
	{
		//To avoid bugs, storyboard and restart buttons are disable during human pawn promotion
		AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
		if (IsValid(PlayerController))
		{
			for (UUI_MoveBox* MoveBox : PlayerController->HUDChess->AllMoves)
			{
				MoveBox->SetIsEnabled(false);
			}
			PlayerController->HUDChess->ResetButtonWidget->SetIsEnabled(false);
		}

		GameMode->bpromotionFlag = true;
		OnPromotionFlagTrue.Broadcast();
	}
	else
	{
		GameMode->bpromotionFlag = false;
		bisMyTurn = false;
		bool MoveResult = GameMode->IsGameEnded(GameMode->ChessBoard->MoveStack.Last(), GameMode->ChessBoard->BlackKing);

		//if the current chessboard state is different from the last move done, the user has clicked a movebutton and replay has to be managed
		if (GameMode->ChessBoard->MoveStack.Num() > 2 && GameMode->ChessBoard->CurrentChessboardState != GameMode->ChessBoard->MoveStack[GameMode->ChessBoard->MoveStack.Num() - 2])
		{
			TArray<ATile*> PreviousColoredTiles = { GameMode->ChessBoard->CurrentChessboardState->To,  GameMode->ChessBoard->CurrentChessboardState->From };
			GameMode->ChessBoard->RestoreSquareColor(PreviousColoredTiles);
			
			if (!ManageReplay(GameMode->ChessBoard->MoveStack.Last()))
			{
				bFirstClick = true;
				bisMyTurn = true;
				return;
			}
		}

		GameMode->ChessBoard->CurrentChessboardState = GameMode->ChessBoard->MoveStack.Last();

		AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
		if (IsValid(PlayerController))
		{
			UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(GameMode->ChessBoard->MoveStack.Last());
			MoveBox->Move = GameMode->ChessBoard->MoveStack.Last();
		}

		if (!MoveResult)
		{
			GameMode->TurnNextPlayer();
		}
	}
}

void AChess_HumanPlayer::OnTurn()
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	AGameField* ChessBoard = GameMode->ChessBoard;
	
	AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PlayerController))
	{
		for (UUI_MoveBox* MoveBox : PlayerController->HUDChess->AllMoves)
		{
			MoveBox->SetIsEnabled(true);
		}
		PlayerController->HUDChess->ResetButtonWidget->SetIsEnabled(true);
	}

	bisMyTurn = true;
	bFirstClick = true;
	actualMoves.Empty();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Your Turn"));
	GameInstance->SetTurnMessage(TEXT("Human Turn"));
}

void AChess_HumanPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win!"));
	GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	GameInstance->IncrementScoreHumanPlayer();
}

void AChess_HumanPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
	//GameInstance->SetTurnMessage(TEXT("Human Loses!"));
}

void AChess_HumanPlayer::OnDraw(EResult DrawOrigin)
{
	if (DrawOrigin == EResult::STALEMATE)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("It's a stalemate!"));
		GameInstance->SetTurnMessage(TEXT("It's a stalemate!"));
	}
}


void AChess_HumanPlayer::OnClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnClick"));
	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	/*VA CONTROLLATO SE IL SEGUENTE CAST VA A BUON FINE*/
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);

	if (IsValid(GameMode) && Hit.bBlockingHit && !Hit.GetActor()->IsHidden() && bisMyTurn && !GameMode->bisGameOver)
	{
		if (bFirstClick)
		{
			CurrPiece = Cast<AChessPiece>(Hit.GetActor());
			if (IsValid(CurrPiece) && CurrPiece->PieceColor == EColor::WHITE)
			{
				if (SelectedTile != nullptr)
				{
					int32 x = SelectedTile->GetGridPosition().X;
					int32 y = SelectedTile->GetGridPosition().Y;
					if (((x + y) % 2) == 0)
					{
						SelectedTile->SetTileColor(0);
					}
					else
					{
						SelectedTile->SetTileColor(1);
					}
				}				

				SelectedTile = CurrPiece->ChessBoard->TileMap[CurrPiece->PlaceAt];
				TArray<ATile*> candidateMoves = CurrPiece->validMoves();
				APawnPiece* CurrPawn = Cast<APawnPiece>(CurrPiece);
				if (IsValid(CurrPawn))
				{
					if (!GameMode->ChessBoard->MoveStack.IsEmpty())
					{
						UMove* PreviousMove = GameMode->ChessBoard->CurrentChessboardState;
						TArray<ATile*> CandidateEnPassant = GameMode->DetectEnPassant(CurrPawn, PreviousMove->PieceMoving, PreviousMove->To, PreviousMove->From);
						for (ATile* Candidate : CandidateEnPassant)
						{
							candidateMoves.Add(Candidate);
						}
					}
				}

				SelectedTile->SetTileColor(2);
				for (ATile* candidateTile : candidateMoves)
				{
					if (CurrPiece->IsLegal(candidateTile))
					{
						actualMoves.Add(candidateTile);
						candidateTile->SetTileColor(3);
					}
				}

				UMove* HumanMove = NewObject<UMove>();
				if (HumanMove)
				{
					GameMode->ChessBoard->MoveStack.Add(HumanMove);
					HumanMove->MoveNumber = GameMode->MoveCounter;
					HumanMove->From = SelectedTile;
					HumanMove->PieceMoving = CurrPiece;
				}
				bFirstClick = false;
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ho rilevato il secondo click"));
			if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
			{
				if (CurrTile->GetTileStatus() == ETileStatus::EMPTY && actualMoves.Contains(CurrTile))
				{
					//ResolveAmbiguitiNotation!!!!(CurrTile)

					APawnPiece* CurrPawn = Cast<APawnPiece>(CurrPiece);
					if (IsValid(CurrPawn))
					{
						if (CurrTile->GetTileStatus() == ETileStatus::EMPTY && CurrTile->GetGridPosition().Y != SelectedTile->GetGridPosition().Y)
						{
							FVector2D OpponentPawnPosition = FVector2D(CurrTile->GetGridPosition().X - 1, CurrTile->GetGridPosition().Y);
							for (AChessPiece* blackPiece : GameMode->ChessBoard->BlackPieceOnChessBoard)
							{
								if (blackPiece->PlaceAt == OpponentPawnPosition)
								{
									GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(blackPiece);
									GameMode->ChessBoard->MoveStack.Last()->benPassant = true;
									GameMode->ChessBoard->MoveStack.Last()->PieceCaptured = blackPiece;
									blackPiece->SetActorHiddenInGame(true);
									GameMode->ChessBoard->MoveOutOfChessBoard(blackPiece);
									break;
								}
							}

							GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(ETileStatus::EMPTY);
						}
					}

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked tile"));
					GameMode->MovePiece(CurrPiece, SelectedTile, CurrTile);
					
					actualMoves.Add(SelectedTile);
					GameMode->ChessBoard->RestoreSquareColor(actualMoves);

					GameMode->ChessBoard->MoveStack.Last()->To = CurrTile;

					if (!GameMode->CheckForPawnPromotion(CurrPiece))
					{
						bisMyTurn = false;
						bool MoveResult = GameMode->IsGameEnded(GameMode->ChessBoard->MoveStack.Last(), GameMode->ChessBoard->BlackKing);

						//Da questo momento in poi la mossa senza promozione e senza cattura é completa
						if (GameMode->ChessBoard->MoveStack.Num() > 2 && GameMode->ChessBoard->CurrentChessboardState != GameMode->ChessBoard->MoveStack[GameMode->ChessBoard->MoveStack.Num() - 2])
						{
							TArray<ATile*> PreviousColoredTiles = { GameMode->ChessBoard->CurrentChessboardState->To,  GameMode->ChessBoard->CurrentChessboardState->From };
							GameMode->ChessBoard->RestoreSquareColor(PreviousColoredTiles);
							//gestione replay
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("REPLAY"));
							if (!ManageReplay(GameMode->ChessBoard->MoveStack.Last()))
							{
								bFirstClick = true;
								bisMyTurn = true;
								return;
							}
						}

						GameMode->ChessBoard->CurrentChessboardState = GameMode->ChessBoard->MoveStack.Last();

						AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
						if (IsValid(PlayerController))
						{
							UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(GameMode->ChessBoard->MoveStack.Last());
							MoveBox->Move = GameMode->ChessBoard->MoveStack.Last();
						}

						if (!MoveResult)
						{
							GameMode->TurnNextPlayer();
						}
					}
					else
					{
						GameMode->ChessBoard->MoveStack.Last()->bisPromotion = true;
					}
				}
			}
			if (AChessPiece* DestinationPiece = Cast<AChessPiece>(Hit.GetActor()))
			{	
				if (DestinationPiece == CurrPiece)
				{
					actualMoves.Add(SelectedTile);
					GameMode->ChessBoard->RestoreSquareColor(actualMoves);
					APawnPiece* PawnInStart = Cast<APawnPiece>(DestinationPiece);
					if (IsValid(PawnInStart) && PawnInStart->PlaceAt.X == 1)
					{
						PawnInStart->bfirstMove = true;
					}
					UMove* MoveToDelete = GameMode->ChessBoard->MoveStack.Last();
					GameMode->ChessBoard->MoveStack.Remove(MoveToDelete);

					// Convert the integer variable to a string
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("IL NUMERO DI MOSSE E':"));
					FString MyIntString = FString::Printf(TEXT("%d"), GameMode->ChessBoard->MoveStack.Num());
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, MyIntString);

					bFirstClick = true;
				}
				if (DestinationPiece->PieceColor == EColor::BLACK)
				{
					ATile* CurrTile = DestinationPiece->ChessBoard->TileMap[DestinationPiece->PlaceAt];
					/*CATTURA DI UN PEZZO*/
					if (actualMoves.Contains(CurrTile))
					{

						

						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked enemy piece"));
						GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(DestinationPiece);
						DestinationPiece->SetActorHiddenInGame(true);
						GameMode->ChessBoard->MoveOutOfChessBoard(DestinationPiece);
						GameMode->MovePiece(CurrPiece, SelectedTile, CurrTile);

						actualMoves.Add(SelectedTile);
						GameMode->ChessBoard->RestoreSquareColor(actualMoves);

						GameMode->ChessBoard->MoveStack.Last()->To = CurrTile;
						GameMode->ChessBoard->MoveStack.Last()->bisCapture = true;
						GameMode->ChessBoard->MoveStack.Last()->PieceCaptured = DestinationPiece;

						if (!GameMode->CheckForPawnPromotion(CurrPiece))
						{
							bisMyTurn = false;
							bool MoveResult = GameMode->IsGameEnded(GameMode->ChessBoard->MoveStack.Last(), GameMode->ChessBoard->BlackKing);
							
							//Da questo momento in poi la mossa senza promozione e con cattura é completa
							if (GameMode->ChessBoard->MoveStack.Num() > 2 && GameMode->ChessBoard->CurrentChessboardState != GameMode->ChessBoard->MoveStack[GameMode->ChessBoard->MoveStack.Num() - 2])
							{
								TArray<ATile*> PreviousColoredTiles = { GameMode->ChessBoard->CurrentChessboardState->To,  GameMode->ChessBoard->CurrentChessboardState->From };
								GameMode->ChessBoard->RestoreSquareColor(PreviousColoredTiles);
								//gestione replay
								GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("REPLAY"));
								if (!ManageReplay(GameMode->ChessBoard->MoveStack.Last()))
								{
									bFirstClick = true;
									bisMyTurn = true;
									return;
								}
							}

							GameMode->ChessBoard->CurrentChessboardState = GameMode->ChessBoard->MoveStack.Last();

							AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
							if (IsValid(PlayerController))
							{
								UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(GameMode->ChessBoard->MoveStack.Last());
								MoveBox->Move = GameMode->ChessBoard->MoveStack.Last();
							}

							if (!MoveResult)
							{
								GameMode->TurnNextPlayer();
							}
						}
						else
						{
							GameMode->ChessBoard->MoveStack.Last()->bisPromotion = true;
						}
					}
				}
			}
		}
	}
}