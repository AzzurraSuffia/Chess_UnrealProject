// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_PlayerInterface.h"
#include "GameField.h"
#include "ChessPiece.h"
#include"RookPiece.h"
#include"PawnPiece.h"
#include"KnightPiece.h"
#include"BishopPiece.h"
#include"QueenPiece.h"
#include"KingPiece.h"
#include "GameFramework/GameModeBase.h"
#include "Chess_GameMode.generated.h"

class AActor;

struct FPosition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);

UENUM()
enum class EResult : uint8
{
	CHECKMATE,
	STALEMATE
};

/**
 * 
 */
UCLASS()
class CHESS_UE52_API AChess_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	//boolean flag to manage human player's pawn promotion
	UPROPERTY(EditAnywhere)
	bool bpromotionFlag = false;

	// tracks if the game is over
	bool bisGameOver;

	// array of player interfaces of all possibile players
	TArray<IChess_PlayerInterface*> AllPlayers;

	// array of player interfaces of the players currently playing
	TArray<IChess_PlayerInterface*> Players;

	// show the start menu
	UPROPERTY(BlueprintAssignable)
	FOnGameStart OnStart;

	// check if player's king is in check
	bool IsKingInCheck(int32 Player);

	//manage end of game based on the result (checkmate or stalemate)
	void ManageEndOfGame(int32 Player, EResult GameResult);

	// check if a player has some legal moves left
	bool PlayerCanMove(int32 Player);

	// check if promotion is possible for CurrPiece
	bool CheckForPawnPromotion(AChessPiece* CurrPiece);

	// move an actor from the departure tile to destination tile
	void MovePiece(AChessPiece* Piece, ATile* From, ATile* To);

	// check if game ended
	bool IsGameEnded(UMove* Move, AKingPiece* King);

	// detect all candidate destination tiles where en passant can be executed
	TArray<ATile*> DetectEnPassant(APawnPiece* Pawn, AChessPiece* PreviousPieceMoving, ATile* PreviousTo, ATile* PreviousFrom);

	// index of the player in players who 
	int32 CurrentPlayer;

	// tracks the number of moves
	int32 MoveCounter;

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGameField> GameFieldClass;

	// field size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FieldSize;

	// reference to a GameField object
	UPROPERTY(VisibleAnywhere)
	AGameField* ChessBoard;

	AChess_GameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// called at the start of the game
	UFUNCTION(BlueprintCallable)
	void ChoosePlayerAndStartGame(bool difficult);

	// get the next player index
	int32 GetNextPlayer(int32 Player);

	// called at the end of the game turn
	void TurnNextPlayer();
};