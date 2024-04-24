// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "GameFramework/Actor.h"
#include "PawnPiece.generated.h"

UCLASS()
class CHESS_UE52_API APawnPiece : public AChessPiece
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APawnPiece();

	//ovverride since pawns capture only diagonally
	virtual bool CanCaptureOpponentPiece(AChessPiece* PieceCaptured) override;

	//return an array of candidate destination tiles from the current position and chessboard state 
	virtual TArray<ATile*> validMoves() override;

	//return piece value for minimax evaluation function
	virtual int32 PieceWeight() override;

	//update data structures to virtually execute a move
	virtual AChessPiece* doVirtualMove(AChessPiece* Piece, ATile* from, ATile* to) override;

	//update data structures to virtually revert a move
	virtual void undoVirtualMove(AChessPiece* Piece, ATile* from, ATile* to, AChessPiece* CapturedPiece) override;

	//boolean flag for pawn first move
	bool bfirstMove;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
