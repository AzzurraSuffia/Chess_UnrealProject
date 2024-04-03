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

	EColor PieceColor;
	FVector2D PlaceAt;
	AGameField* ChessBoard;

	virtual bool CanCaptureOpponentPiece(AChessPiece* PieceCaptured);

	virtual TArray<ATile*> validMoves();

	virtual bool IsLegal(ATile* to);
	virtual	AChessPiece* doVirtualMove(AChessPiece* Piece, ATile* from, ATile* to);
	virtual	void undoVirtualMove(AChessPiece* Piece, ATile* from, ATile* to, AChessPiece* CapturedPiece);

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
