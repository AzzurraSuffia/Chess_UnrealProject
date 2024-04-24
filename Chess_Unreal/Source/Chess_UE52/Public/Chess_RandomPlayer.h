// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_PlayerInterface.h"
#include "GameFramework/Pawn.h"
#include "Chess_GameInstance.h"
#include "Chess_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Chess_RandomPlayer.generated.h"

UCLASS()
class CHESS_UE52_API AChess_RandomPlayer : public APawn, public IChess_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChess_RandomPlayer();

	UChess_GameInstance* GameInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//random player behavior when one of his pawn are promoted
	virtual void OnPawnPromotion() override;

	//random player behavior when his turn comes
	virtual void OnTurn() override;

	//random player behavior when he wins
	virtual void OnWin() override;

	//random player behavior when he loses
	virtual void OnLose() override;

	//random player behavior when the game ends in a draw
	virtual void OnDraw(EResult DrawOrigin) override;
};
