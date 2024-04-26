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
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	//move clicked in the storyboard was a black move: the human move is valid (confirm the move)
	if (GameMode->ChessBoard->CurrentChessboardState->PieceMoving->PieceColor == EColor::BLACK)
	{
		//restore squares colors of departure and destination tiles of the clicked move
		GameMode->ChessBoard->RestoreASquareColor(GameMode->ChessBoard->CurrentChessboardState->To);
		GameMode->ChessBoard->RestoreASquareColor(GameMode->ChessBoard->CurrentChessboardState->From);

		//get move indexes in move stack
		int32 MoveDoneIdx = GameMode->ChessBoard->MoveStack.Find(GameMode->ChessBoard->MoveStack.Last());
		int32 ClickedMoveIdx = GameMode->ChessBoard->MoveStack.Find(GameMode->ChessBoard->CurrentChessboardState);

		if (ClickedMoveIdx != INDEX_NONE && MoveDoneIdx != INDEX_NONE)
		{
			AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
			if (IsValid(PlayerController))
			{
				//update game mode move counter
				GameMode->MoveCounter = GameMode->ChessBoard->MoveStack[ClickedMoveIdx]->MoveNumber + 1;
				GameMode->ChessBoard->MoveStack.Last()->MoveNumber = GameMode->MoveCounter;

				//destroy text boxes between last move and clicked move (one text box per move)
				int32 LastTextBoxToPreserve = ClickedMoveIdx/2;
				int32 LastTextBoxIdx = PlayerController->HUDChess->OtherNotationComponents.Num() - 1;

				for (int32 i = LastTextBoxIdx; i > LastTextBoxToPreserve; i--)
				{
					PlayerController->HUDChess->OtherNotationComponents[i]->ConditionalBeginDestroy();
				}
				//remove the references to the text boxes destroyed
				PlayerController->HUDChess->OtherNotationComponents.SetNum(LastTextBoxToPreserve+1);

				
				for (int32 i = MoveDoneIdx - 1; i > ClickedMoveIdx; i--)
				{
					UMove* Move = GameMode->ChessBoard->MoveStack[i];

					//destroy all the new piece from promotion in the moves between last move and clicked move
					if (Move->bisPromotion)
					{
						Move->PiecePromoted->Destroy();
					}
					//destroy all the moves between last move and clicked move
					Move->ConditionalBeginDestroy();

					//remove them from move stack
					GameMode->ChessBoard->MoveStack.Remove(Move);

					//destroy all the move boxes between last move and clicked move
					PlayerController->HUDChess->AllMoves[i]->ConditionalBeginDestroy();
				}
				//remove the references to the move boxes destroyed
				PlayerController->HUDChess->AllMoves.SetNum(ClickedMoveIdx+1);
			}
		}
		return true;
	}

	//move clicked in the storyboard was a white move: the human move is not valid (undo the move)
	else
	{
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

		//set replay color again
		GameMode->ChessBoard->CurrentChessboardState->To->SetTileColor(5);
		GameMode->ChessBoard->CurrentChessboardState->From->SetTileColor(5);

		return false;
	}
}

TArray<ATile*> AChess_HumanPlayer::GetActualMoves() const
{
	return actualMoves;
}

ATile* AChess_HumanPlayer::GetSelectedTile() const
{
	return SelectedTile;
}

bool AChess_HumanPlayer::GetFirstClick() const
{
	return bFirstClick;
}

void AChess_HumanPlayer::SetFirstClick(const bool flag)
{
	bFirstClick = flag;
}

void AChess_HumanPlayer::SetActualMovesEmpty()
{
	actualMoves.Empty();
}

void AChess_HumanPlayer::SetSelectedTileNull()
{
	SelectedTile = nullptr;
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
* When bpromotionFlag is true, OnPawnPromotion() checks if game ended and add the move widget to the storyboard (human pawn promotion end)
*/
void AChess_HumanPlayer::OnPawnPromotion()
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode->bpromotionFlag)
	{
		//To avoid bugs, storyboard and restart buttons are disabled during human pawn promotion
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

		//show pawn promotion menu
		OnPromotionFlagTrue.Broadcast();
	}
	else
	{
		GameMode->bpromotionFlag = false;
		bisMyTurn = false;

		//check if game ended with this move
		bool MoveResult = GameMode->IsGameEnded(GameMode->ChessBoard->MoveStack.Last(), GameMode->ChessBoard->BlackKing);

		//if the current chessboard state is different from the last move done, the user has clicked a movebutton and replay has to be managed
		if (GameMode->ChessBoard->MoveStack.Num() > 2)
		{
			if (GameMode->ChessBoard->CurrentChessboardState != GameMode->ChessBoard->MoveStack[GameMode->ChessBoard->MoveStack.Num() - 2])
			{
				if (!ManageReplay(GameMode->ChessBoard->MoveStack.Last()))
				{
					//if the current chessboard state is different from the last move done, but replay is not possible
					//give human player the chance again to click
					bFirstClick = true;
					bisMyTurn = true;
					return;
				}
			}
			else
			{
				//if the human player clicked on the last move button, do not execute replay routine, just restore tile colors
				GameMode->ChessBoard->RestoreASquareColor(GameMode->ChessBoard->CurrentChessboardState->To);
				GameMode->ChessBoard->RestoreASquareColor(GameMode->ChessBoard->CurrentChessboardState->From);
			}
		}

		//update the current chessboard state with this move
		GameMode->ChessBoard->CurrentChessboardState = GameMode->ChessBoard->MoveStack.Last();

		//add the move button of this move to the storyboard
		AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
		if (IsValid(PlayerController))
		{
			UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(GameMode->ChessBoard->MoveStack.Last());
			MoveBox->Move = GameMode->ChessBoard->MoveStack.Last();
		}

		//if the game isn't ended yet, it's next player's turn
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

	//enable move buttons and restart button
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
	SelectedTile = nullptr;
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
	
	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);

	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);

	if (IsValid(GameMode) && Hit.bBlockingHit && !Hit.GetActor()->IsHidden() && bisMyTurn && !GameMode->bisGameOver)
	{
		//first click
		if (bFirstClick)
		{
			CurrPiece = Cast<AChessPiece>(Hit.GetActor());
			if (IsValid(CurrPiece) && CurrPiece->PieceColor == EColor::WHITE)
			{
				//get the tile below the selected piece
				SelectedTile = CurrPiece->ChessBoard->TileMap[CurrPiece->PlaceAt];

				//get the candidate destination tiles for the selected piece
				TArray<ATile*> candidateMoves = CurrPiece->validMoves();

				//if it's a pawn, check if en passant is possible
				APawnPiece* CurrPawn = Cast<APawnPiece>(CurrPiece);
				if (IsValid(CurrPawn))
				{
					if (!GameMode->ChessBoard->MoveStack.IsEmpty())
					{
						UMove* PreviousMove = GameMode->ChessBoard->CurrentChessboardState;
						TArray<ATile*> CandidateEnPassant = GameMode->DetectEnPassant(CurrPawn, PreviousMove->PieceMoving, PreviousMove->To, PreviousMove->From);
						
						//add each en passant destination tile found to the other tiles
						for (ATile* Candidate : CandidateEnPassant)
						{
							candidateMoves.Add(Candidate);
						}
					}
				}

				SelectedTile->SetTileColor(2);

				//from candidate destination tiles find the legal ones
				for (ATile* candidateTile : candidateMoves)
				{
					if (CurrPiece->IsLegal(candidateTile))
					{
						actualMoves.Add(candidateTile);
						candidateTile->SetTileColor(3);
					}
				}

				//create and update Move UObject with the data of this move
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

		//second click
		else
		{
			//if a tile is clicked
			if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
			{
				if (CurrTile->GetTileStatus() == ETileStatus::EMPTY && actualMoves.Contains(CurrTile))
				{
					
					APawnPiece* CurrPawn = Cast<APawnPiece>(CurrPiece);
					if (IsValid(CurrPawn))
					{
						//if it's a pawn and the clicked tile has a different y coordinate, en passant has to be managed 
						if (CurrTile->GetTileStatus() == ETileStatus::EMPTY && CurrTile->GetGridPosition().Y != SelectedTile->GetGridPosition().Y)
						{
							//get opponent's pawn position
							FVector2D OpponentPawnPosition = FVector2D(CurrTile->GetGridPosition().X - 1, CurrTile->GetGridPosition().Y);
							
							//get opponent'spawn reference
							for (AChessPiece* blackPiece : GameMode->ChessBoard->BlackPieceOnChessBoard)
							{
								if (blackPiece->PlaceAt == OpponentPawnPosition)
								{
									//update move object
									GameMode->ChessBoard->MoveStack.Last()->benPassant = true;
									GameMode->ChessBoard->MoveStack.Last()->PieceCaptured = blackPiece;

									//remove the captured pawn from chessboard and set it hidden
									GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(blackPiece);
									blackPiece->SetActorHiddenInGame(true);
									GameMode->ChessBoard->MoveOutOfChessBoard(blackPiece);
									break;
								}
							}

							GameMode->ChessBoard->TileMap[OpponentPawnPosition]->SetTileStatus(ETileStatus::EMPTY);
						}
					}

					GameMode->MovePiece(CurrPiece, SelectedTile, CurrTile);

					//restore suggested tiles color
					actualMoves.Add(SelectedTile);
					GameMode->ChessBoard->RestoreSquaresColor(actualMoves);

					GameMode->ChessBoard->MoveStack.Last()->To = CurrTile;

					//if pawn promotion does not occur
					if (!GameMode->CheckForPawnPromotion(CurrPiece))
					{
						bisMyTurn = false;

						//check if game ended with this move
						bool MoveResult = GameMode->IsGameEnded(GameMode->ChessBoard->MoveStack.Last(), GameMode->ChessBoard->BlackKing);

						//if the current chessboard state is different from the last move done, the user has clicked a movebutton and replay has to be managed
						if (GameMode->ChessBoard->MoveStack.Num() > 2)
						{
							if (GameMode->ChessBoard->CurrentChessboardState != GameMode->ChessBoard->MoveStack[GameMode->ChessBoard->MoveStack.Num() - 2])
							{
								if (!ManageReplay(GameMode->ChessBoard->MoveStack.Last()))
								{
									//if the current chessboard state is different from the last move done, but replay is not possible
									//give human player the chance again to click
									bFirstClick = true;
									bisMyTurn = true;
									return;
								}
							}
							else
							{
								//if the human player clicked on the last move button, do not execute replay routine, just restore tile colors
								GameMode->ChessBoard->RestoreASquareColor(GameMode->ChessBoard->CurrentChessboardState->To);
								GameMode->ChessBoard->RestoreASquareColor(GameMode->ChessBoard->CurrentChessboardState->From);
							}
						}

						//update the current chessboard state with this move
						GameMode->ChessBoard->CurrentChessboardState = GameMode->ChessBoard->MoveStack.Last();

						//add the move button of this move to the storyboard
						AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
						if (IsValid(PlayerController))
						{
							UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(GameMode->ChessBoard->MoveStack.Last());
							MoveBox->Move = GameMode->ChessBoard->MoveStack.Last();
						}

						//if the game isn't ended yet, it's next player's turn
						if (!MoveResult)
						{
							GameMode->TurnNextPlayer();
						}
					}
					else
					{
						//if pawn promotion occurs, update move object
						GameMode->ChessBoard->MoveStack.Last()->bisPromotion = true;
					}
				}
			}

			
			if (AChessPiece* DestinationPiece = Cast<AChessPiece>(Hit.GetActor()))
			{	
				//if the selected piece is clicked
				if (DestinationPiece == CurrPiece)
				{
					actualMoves.Add(SelectedTile);
					GameMode->ChessBoard->RestoreSquaresColor(actualMoves);

					//restore first move flag for pawns if needed
					APawnPiece* PawnInStart = Cast<APawnPiece>(DestinationPiece);
					if (IsValid(PawnInStart) && PawnInStart->PlaceAt.X == 1)
					{
						PawnInStart->bfirstMove = true;
					}

					//delete the instantiated move
					UMove* MoveToDelete = GameMode->ChessBoard->MoveStack.Last();
					GameMode->ChessBoard->MoveStack.Remove(MoveToDelete);

					bFirstClick = true;
					CurrPiece = nullptr;
					actualMoves.Empty();
					SelectedTile = nullptr;

				}

				//if an opponent's piece is clicked
				if (DestinationPiece->PieceColor == EColor::BLACK)
				{
					ATile* CurrTile = DestinationPiece->ChessBoard->TileMap[DestinationPiece->PlaceAt];
					
					//if the tile below the opponent's piece is a legal tile
					if (actualMoves.Contains(CurrTile))
					{
						actualMoves.Add(SelectedTile);
						GameMode->ChessBoard->RestoreSquaresColor(actualMoves);

						//remove the captured piece from chessboard
						GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(DestinationPiece);
						DestinationPiece->SetActorHiddenInGame(true);
						GameMode->ChessBoard->MoveOutOfChessBoard(DestinationPiece);

						GameMode->MovePiece(CurrPiece, SelectedTile, CurrTile);

						//update move object
						GameMode->ChessBoard->MoveStack.Last()->To = CurrTile;
						GameMode->ChessBoard->MoveStack.Last()->bisCapture = true;
						GameMode->ChessBoard->MoveStack.Last()->PieceCaptured = DestinationPiece;

						//if pawn promotion does not occur
						if (!GameMode->CheckForPawnPromotion(CurrPiece))
						{
							bisMyTurn = false;

							//check if game ended with this move
							bool MoveResult = GameMode->IsGameEnded(GameMode->ChessBoard->MoveStack.Last(), GameMode->ChessBoard->BlackKing);
							
							//if the current chessboard state is different from the last move done, the user has clicked a movebutton and replay has to be managed
							if (GameMode->ChessBoard->MoveStack.Num() > 2)
							{
								if (GameMode->ChessBoard->CurrentChessboardState != GameMode->ChessBoard->MoveStack[GameMode->ChessBoard->MoveStack.Num() - 2])
								{
									if (!ManageReplay(GameMode->ChessBoard->MoveStack.Last()))
									{
										//if the current chessboard state is different from the last move done, but replay is not possible
										//give human player the chance again to click
										bFirstClick = true;
										bisMyTurn = true;
										return;
									}
								}
								else
								{
									//if the human player clicked on the last move button, do not execute replay routine, just restore tile colors
									GameMode->ChessBoard->RestoreASquareColor(GameMode->ChessBoard->CurrentChessboardState->To);
									GameMode->ChessBoard->RestoreASquareColor(GameMode->ChessBoard->CurrentChessboardState->From);
								}
							}

							//update the current chessboard state with this move
							GameMode->ChessBoard->CurrentChessboardState = GameMode->ChessBoard->MoveStack.Last();

							//add the move button of this move to the storyboard
							AChess_PlayerController* PlayerController = Cast<AChess_PlayerController>(GetWorld()->GetFirstPlayerController());
							if (IsValid(PlayerController))
							{
								UUI_MoveBox* MoveBox = PlayerController->HUDChess->AddMoveWidget(GameMode->ChessBoard->MoveStack.Last());
								MoveBox->Move = GameMode->ChessBoard->MoveStack.Last();
							}

							//if the game isn't ended yet, it's next player's turn
							if (!MoveResult)
							{
								GameMode->TurnNextPlayer();
							}
						}
						else
						{
							//if pawn promotion occurs, update move object
							GameMode->ChessBoard->MoveStack.Last()->bisPromotion = true;
						}
					}
				}
			}
		}
	}
}