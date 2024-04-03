// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "KingPiece.h"
#include "QueenPiece.h"
#include "BishopPiece.h"
#include "KnightPiece.h"
#include "RookPiece.h"
#include "PawnPiece.h"
#include "Move.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"

// macro declaration for a dynamic multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);


UENUM()
enum class EPieceNotation : uint8
{
	K,
	Q,
	B,
	N,
	R,
	P,
	EMPTY
};

UCLASS()
class CHESS_UE52_API AGameField : public AActor
{
	GENERATED_BODY()

public:
	// keeps track of tiles
	UPROPERTY(Transient)
	TArray<ATile*> TileArray;

	//given a position returns a tile
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

	UPROPERTY(Transient)
	TArray<UMove*> MoveStack;

	// array of white piece still on chessboard
	TArray<AChessPiece*> WhitePieceOnChessBoard;

	// array of black piece still on chessboard
	TArray<AChessPiece*> BlackPieceOnChessBoard;

	AKingPiece* WhiteKing;

	AKingPiece* BlackKing;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	// float NormalizedCellPadding;

	static const int32 NOT_ASSIGNED = -1;

	// BlueprintAssignable Usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	UPROPERTY(BlueprintAssignable)
	FOnReset OnResetEvent;

	// size of field
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/*
	// size of pieces
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PieceSize;
	*/

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AKingPiece> KingClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AQueenPiece> QueenClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABishopPiece> BishopClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AKnightPiece> KnightClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARookPiece> RookClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawnPiece> PawnClass;

	// tile padding dimension
	// 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// 	float CellPadding;

	// tile size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize;

	// Sets default values for this actor's properties
	AGameField();

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// remove all signs from the field
	UFUNCTION(BlueprintCallable)
	void ResetField();

	UFUNCTION(BlueprintCallable)
	FString GetLastMoveAlgebricNotation();

	UFUNCTION(BlueprintCallable)
	ARookPiece* SpawnPieceRook(EColor Color, const double InX, const double InY, const float TileScale);

	UFUNCTION(BlueprintCallable)
	AKnightPiece* SpawnPieceKnight(EColor Color, const double InX, const double InY, const float TileScale);

	UFUNCTION(BlueprintCallable)
	ABishopPiece* SpawnPieceBishop(EColor Color, const double InX, const double InY, const float TileScale);

	UFUNCTION(BlueprintCallable)
	AQueenPiece* SpawnPieceQueen(EColor Color, const double InX, const double InY, const float TileScale);

	AKingPiece* SpawnPieceKing(EColor Color, const double InX, const double InY, const float TileScale);
	APawnPiece* SpawnPiecePawn(EColor Color, const double InX, const double InY, const float TileScale);

	// generate an empty game field
	void GenerateField();

	UFUNCTION(BlueprintCallable)
	void PromotePawn(EPieceNotation ToPromote);

	void MoveOutOfChessBoard(AChessPiece* HiddenPiece, bool flag);

	// return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit);

	// return the array of tile pointers
	TArray<ATile*>& GetTileArray();

	// return a relative position given (x,y) position
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;
	
	//public:	
	//// Called every frame
	//	virtual void Tick(float DeltaTime) override;

};