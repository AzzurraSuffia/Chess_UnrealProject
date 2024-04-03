// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "GameFramework/Actor.h"
#include "KingPiece.generated.h"

UCLASS()
class CHESS_UE52_API AKingPiece : public AChessPiece
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKingPiece();

	/*
	bool isStaleMate(Square Cells[][8], int x, int y);
    bool isCheck(Square Cells[][8], int x, int y);
    bool isCheckmate(Square Cells[][8], int x, int y);
	*/

	virtual TArray<ATile*> validMoves() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
