// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "GameFramework/Actor.h"
#include "QueenPiece.generated.h"

UCLASS(Abstract)
class CHESS_UE52_API AQueenPiece : public AChessPiece
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQueenPiece();

	//return an array of candidate destination tiles from the current position and chessboard state 
	virtual TArray<ATile*> validMoves() override;

	//return piece value for minimax evaluation function
	virtual int32 PieceWeight() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
