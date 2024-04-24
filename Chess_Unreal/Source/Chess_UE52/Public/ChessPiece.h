// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessPiece.generated.h"

class AGameField;
class ATile;

UENUM()
enum class EColor : uint8
{
	WHITE,
	BLACK,
	EMPTY
};

UCLASS(Abstract)
class CHESS_UE52_API AChessPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessPiece();

	//color of the piece
	EColor PieceColor;

	//piece location
	FVector2D PlaceAt;

	//reference to the chessboard
	AGameField* ChessBoard;

	//check if this piece can capture another one based on the current chessboard state
	virtual bool CanCaptureOpponentPiece(AChessPiece* PieceCaptured);

	//return piece value for minimax evaluation function
	virtual int32 PieceWeight();

	//return an array of candidate destination tiles from the current position and chessboard state 
	virtual TArray<ATile*> validMoves();

	//find out if a move is legal or not
	virtual bool IsLegal(ATile* to);

	//update data structures to virtually execute a move
	virtual	AChessPiece* doVirtualMove(AChessPiece* Piece, ATile* from, ATile* to);

	//update data structures to virtually revert a move
	virtual	void undoVirtualMove(AChessPiece* Piece, ATile* from, ATile* to, AChessPiece* CapturedPiece);

	//set the color of the piece
	void SetPieceColor(EColor PieceColor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMaterialInstance* BlackPiece;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMaterialInstance* WhitePiece;
	
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
