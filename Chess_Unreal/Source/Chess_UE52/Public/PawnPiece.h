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

	virtual bool CanCaptureOpponentPiece(AChessPiece* PieceCaptured) override;

	//return an array of candidate destination tiles from the current position and chessboard state 
	virtual TArray<ATile*> validMoves() override;

	//return piece value for minimax evaluation function
	virtual int32 PieceWeight() override;

	virtual AChessPiece* doVirtualMove(AChessPiece* Piece, ATile* from, ATile* to) override;
	virtual void undoVirtualMove(AChessPiece* Piece, ATile* from, ATile* to, AChessPiece* CapturedPiece) override;

	bool bfirstMove;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
