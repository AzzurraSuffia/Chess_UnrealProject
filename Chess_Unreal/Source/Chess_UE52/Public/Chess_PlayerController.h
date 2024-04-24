// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Chess_HumanPlayer.h"
#include "Blueprint/UserWidget.h"
#include "InputActionValue.h"
#include "HUD_Chess.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Chess_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_UE52_API AChess_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AChess_PlayerController();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* ChessContext;


	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ClickAction;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditAnywhere, Category = "Child Widgets")
	TSubclassOf<UUserWidget> HUDWidget;

	//reference to the widget
	UHUD_Chess* HUDChess;

	void ClickOnGrid();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	
};
