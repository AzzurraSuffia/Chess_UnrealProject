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

	virtual TArray<ATile*> validMoves() override;

	virtual int32 PieceWeight() override;

	bool bfirstMove;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
