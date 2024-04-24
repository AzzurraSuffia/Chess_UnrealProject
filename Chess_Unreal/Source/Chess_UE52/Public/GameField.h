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
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"

// Macro declaration for a dynamic multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);

// Enum of pieces capital letter
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
	// Keeps track of tiles
	UPROPERTY(Transient)
	TArray<ATile*> TileArray;

	// Given a position returns a tile
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

	// Temporally sorted array of game moves
	UPROPERTY(Transient)
	TArray<UMove*> MoveStack;

	// Array of white piece still on chessboard
	TArray<AChessPiece*> WhitePieceOnChessBoard;

	// Array of black piece still on chessboard
	TArray<AChessPiece*> BlackPieceOnChessBoard;

	// Reference to white king
	AKingPiece* WhiteKing;

	// Reference to black king
	AKingPiece* BlackKing;

	// Last move of move stack whose result is currently displayed on chessboard
	UMove* CurrentChessboardState = nullptr;

	static const int32 NOT_ASSIGNED = -1;

	// BlueprintAssignable Usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	UPROPERTY(BlueprintAssignable)
	FOnReset OnResetEvent;

	// Size of field
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Size;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClass;

	// TSubclassOf template class King
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AKingPiece> KingClass;

	// TSubclassOf template class Queen
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AQueenPiece> QueenClass;

	// TSubclassOf template class Bishop
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABishopPiece> BishopClass;

	// TSubclassOf template class Knight
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AKnightPiece> KnightClass;

	// TSubclassOf template class Rook
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARookPiece> RookClass;

	// TSubclassOf template class Pawn
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawnPiece> PawnClass;

	// Tile size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize;

	// Sets default values for this actor's properties
	AGameField();

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Remove all pieces from the field
	UFUNCTION(BlueprintCallable)
	void ResetField();

	// Spawn a rook piece in the indicated position
	UFUNCTION(BlueprintCallable)
	ARookPiece* SpawnPieceRook(EColor Color, const double InX, const double InY, const float TileScale);

	// Spawn a knight piece in the indicated position
	UFUNCTION(BlueprintCallable)
	AKnightPiece* SpawnPieceKnight(EColor Color, const double InX, const double InY, const float TileScale);

	// Spawn a bishop piece in the indicated position
	UFUNCTION(BlueprintCallable)
	ABishopPiece* SpawnPieceBishop(EColor Color, const double InX, const double InY, const float TileScale);

	// Spawn a queen piece in the indicated position
	UFUNCTION(BlueprintCallable)
	AQueenPiece* SpawnPieceQueen(EColor Color, const double InX, const double InY, const float TileScale);

	// Spawn a king piece in the indicated position
	AKingPiece* SpawnPieceKing(EColor Color, const double InX, const double InY, const float TileScale);

	// Spawn a pawn piece in the indicated position
	APawnPiece* SpawnPiecePawn(EColor Color, const double InX, const double InY, const float TileScale);

	// Generate an empty game field
	void GenerateField();

	// Promote the pawn in the piece passed as an argument
	UFUNCTION(BlueprintCallable)
	void PromotePawn(EPieceNotation ToPromote);

	// Move a piece out of chessboard
	void MoveOutOfChessBoard(AChessPiece* HiddenPiece);

	// Restore tile color of an array
	void RestoreSquaresColor(TArray<ATile*> Squares);

	// Restore tile color of a single tile
	void RestoreASquareColor(ATile* Square);

	// Return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit);

	// Return the array of tile pointers
	TArray<ATile*>& GetTileArray();

	// Return a relative position given (x,y) position
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// Return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;
	
	//public:	
	//// Called every frame
	//	virtual void Tick(float DeltaTime) override;

};