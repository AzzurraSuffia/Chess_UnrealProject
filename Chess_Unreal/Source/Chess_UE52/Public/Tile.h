// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Tile.generated.h"

UENUM()
enum class ETileStatus : uint8
{
	EMPTY UMETA(DisplayName = "Empty"),
	BLACKPIECE UMETA(DisplayName = "BlackPiece"),
	WHITEPIECE UMETA(DisplayName = "WhitePiece")
};

UCLASS()
class CHESS_UE52_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

	// set the player owner and the status of a tile
	void SetTileStatus(const ETileStatus TileStatus);

	// get the tile status
	ETileStatus GetTileStatus();

	// set the (x, y) position
	void SetGridPosition(const double InX, const double InY);

	// get the (x, y) position
	FVector2D GetGridPosition();

	// set tile material
	void SetTileColor(int32 color);

	// set a number as text of tile text number component 
	void SetTileNumber(int32 number);

	// set a FString as text of tile text letter component 
	void SetTileLetter(FString letter);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	// material instance for black tiles
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMaterialInstance* BlackTile;

	// material instance for white tiles
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMaterialInstance* WhiteTile;

	// material instance for the tile selected by human player on his turn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMaterialInstance* SelectedTile;

	// material instance for suggested tiles during human player's turn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMaterialInstance* SuggestedTile;

	// material instance for kings' tiles when checkmate occurs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMaterialInstance* CheckTile;

	// material instance for departure and destination tiles of the move clicked in the storyboard
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UMaterialInstance* ReplayTile;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* TextNumberComponent;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* TextLetterComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETileStatus Status;

	// (x, y) position of the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D TileGridPosition;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};