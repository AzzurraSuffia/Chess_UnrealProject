// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Chess_PlayerInterface.h"
#include "Chess_GameInstance.h"
#include "Chess_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Chess_MinimaxPlayer.generated.h"

UCLASS()
class CHESS_UE52_API AChess_MinimaxPlayer : public APawn, public IChess_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChess_MinimaxPlayer();

	UChess_GameInstance* GameInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//minimax player behavior when his turn comes
	virtual void OnTurn() override;

	//minimax player behavior when he wins
	virtual void OnWin() override;

	//minimax player behavior when he loses
	virtual void OnLose() override;

	//minimax player behavior when the game ends in a draw
	virtual void OnDraw(EResult DrawOrigin) override;

	//evaluation function for minimax
	int32 EvaluateChessboard(TArray<AChessPiece*>& WhitePieces, TArray<AChessPiece*>& BlackPieces);

	//utility function for minimax
	int32 Utility(int32 Player);

	//alpha-beta minimax 
	int32 AlphaBetaMiniMax(int32 Depth, bool bisMax, int32 alpha, int32 beta, AChessPiece* PreviousPiece, ATile* PreviousTo, ATile* PreviousFrom);

	//find best move for minimax player
	UMove* FindBestMove(AGameField* ChessBoard, int32 Depth);
};
