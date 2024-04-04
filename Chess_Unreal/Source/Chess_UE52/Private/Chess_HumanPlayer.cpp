// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_HumanPlayer.h"
#include "GameField.h"
#include "Chess_GameMode.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Chess_PlayerController.h"

// Sets default values
AChess_HumanPlayer::AChess_HumanPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// create a camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//set the camera as RootComponent
	SetRootComponent(Camera);
	// get the game instance reference
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	// default init values
	PlayerNumber = -1;
	Set = ESet::EMPTY;

}

void ResolveAmbiguityNotation(UMove* Move, AChessPiece* PieceMoving, TArray<AChessPiece*>& MyPieces, ATile* Square, AGameField* ChessBoard)
{
	APawnPiece* Pawn = Cast<APawnPiece>(PieceMoving);
	if (IsValid(Pawn)) 
	{
		for (AChessPiece* Piece : MyPieces)
		{
			APawnPiece* OtherPawn = Cast<APawnPiece>(Piece);
			if (IsValid(OtherPawn) && OtherPawn != Pawn)
			{
				TArray<ATile*> candidateMoves = OtherPawn->validMoves();
				for (ATile* candidateSquare : candidateMoves)
				{
					if (OtherPawn->IsLegal(candidateSquare) && candidateSquare == Square)
					{
						if (candidateSquare->GetGridPosition().Y == Square->GetGridPosition().Y)
						{
							Move->brankAmbiguity = true;
						}
						else
						{
							Move->bfileAmbiguity = true;
						}
					}
				}
			}
		}
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

void AChess_HumanPlayer::OnPawnPromotion()
{
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode->bpromotionFlag)
	{
		GameMode->bpromotionFlag = true;
		OnPromotionFlagTrue.Broadcast();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Human Pawn Promotion"));
		GameInstance->SetTurnMessage(TEXT("Human Pawn Promotion"));
	}
	else
	{
		GameMode->bpromotionFlag = false;
		bisMyTurn = false;
		bool MoveResult = GameMode->IsGameEnded(GameMode->ChessBoard->MoveStack.Last(), GameMode->ChessBoard->BlackKing);

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
	/*DEVO CONTROLLARE QUI SE CI SONO ANCORA MOSSE CHE IL GIOCATORE UMANO PUO' FARE*/
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	AGameField* ChessBoard = GameMode->ChessBoard;
	//METODO 1
	
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

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked piece"));
				SelectedTile = CurrPiece->ChessBoard->TileMap[CurrPiece->PlaceAt];
				TArray<ATile*> candidateMoves = CurrPiece->validMoves();
				SelectedTile->SetTileColor(2);
				for (ATile* candidateTile : candidateMoves)
				{
					if (CurrPiece->IsLegal(candidateTile))
					{
						actualMoves.Add(candidateTile);
						candidateTile->SetTileColor(3);
					}
				}
				if (!actualMoves.IsEmpty())
				{
					GameInstance->SetTurnMessage(TEXT("Human Turn"));
					bFirstClick = false;
					UMove* HumanMove = NewObject<UMove>();
					if (HumanMove)
					{
						GameMode->ChessBoard->MoveStack.Add(HumanMove);
						HumanMove->MoveNumber = GameMode->MoveCounter;
						HumanMove->From = SelectedTile;
						HumanMove->PieceMoving = CurrPiece;
					}
				}
				else
				{
					GameInstance->SetTurnMessage(TEXT("No Moves Available for this Piece"));
				}
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

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked tile"));
					GameMode->MovePiece(CurrPiece, SelectedTile, CurrTile);
					
					actualMoves.Add(SelectedTile);
					GameMode->ChessBoard->RestoreSquareColor(actualMoves);

					GameMode->ChessBoard->MoveStack.Last()->To = CurrTile;

					if (!GameMode->CheckForPawnPromotion(CurrPiece))
					{
						bisMyTurn = false;
						bool MoveResult = GameMode->IsGameEnded(GameMode->ChessBoard->MoveStack.Last(), GameMode->ChessBoard->BlackKing);

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
				if (DestinationPiece->PieceColor == EColor::BLACK)
				{
					ATile* CurrTile = DestinationPiece->ChessBoard->TileMap[DestinationPiece->PlaceAt];
					/*CATTURA DI UN PEZZO*/
					if (actualMoves.Contains(CurrTile))
					{

						//ResolveAmbiguitiNotation!!!!(CurrTile)

						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked enemy piece"));
						GameMode->ChessBoard->BlackPieceOnChessBoard.Remove(DestinationPiece);
						//DestinationPiece->SetActorHiddenInGame(true);
						GameMode->ChessBoard->MoveOutOfChessBoard(DestinationPiece, false);
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