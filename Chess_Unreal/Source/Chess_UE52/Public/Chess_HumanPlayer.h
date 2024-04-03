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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Keeps track of turn
	bool bisMyTurn = false;
	bool bFirstClick = true;
	AChessPiece* CurrPiece = nullptr;
	ATile* SelectedTile = nullptr;
	TArray<ATile*> actualMoves = {};

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnPawnPromotion() override; 
	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;
	virtual void OnDraw(EResult DrawOrigin) override;

	// Called on left mouse click (binding)
	UFUNCTION()
	void OnClick();
};
