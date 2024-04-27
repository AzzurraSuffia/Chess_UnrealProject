// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Chess_GameInstance.h"
#include "ChessPiece.h"
#include "Chess_PlayerInterface.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Chess_HumanPlayer.generated.h"

class UMove;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPromotionFlagTrue);

UCLASS()
class CHESS_UE52_API AChess_HumanPlayer : public APawn, public IChess_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChess_HumanPlayer();

	UPROPERTY(BlueprintAssignable)
	FOnPromotionFlagTrue OnPromotionFlagTrue;

	// Camera component attacched to player pawn
	UCameraComponent* Camera;

	// Game instance reference
	UChess_GameInstance* GameInstance;
	
	//get actual moves
	TArray<ATile*> GetActualMoves() const;

	//get selected tile
	ATile* GetSelectedTile() const;

	//get first click
	bool GetFirstClick() const;

	//set first click
	void SetFirstClick(const bool flag);

	//empty actual moves array
	void SetActualMovesEmpty();

	//set selected tile nullptr
	void SetSelectedTileNull();

	//get CurrPiece
	AChessPiece* GetCurrentPiece() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Keeps track of turn
	bool bisMyTurn = false;

	//current selected piece
	AChessPiece* CurrPiece = nullptr;
	
	//legal moves for the selected piece
	TArray<ATile*> actualMoves = {};

	//tile below selected piece
	ATile* SelectedTile = nullptr;

	//discriminate among human first or second click on chessboard
	bool bFirstClick = true;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//human player routine when one of his pawn is promoted
	virtual void OnPawnPromotion() override; 

	//human player routine when his turn comes
	virtual void OnTurn() override;

	//human player routine when he wins
	virtual void OnWin() override;

	//human player routine when he loses
	virtual void OnLose() override;

	//human player routine when the game ends in a draw
	virtual void OnDraw(EResult DrawOrigin) override;

	// Called on left mouse click (binding)
	UFUNCTION()
	void OnClick();

	bool ManageReplay(UMove* FirstReplayMove);
};
