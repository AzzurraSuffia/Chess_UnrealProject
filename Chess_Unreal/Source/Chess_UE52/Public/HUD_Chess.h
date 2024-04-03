// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "UI_MoveBox.h"
#include "Move.h"
#include "Components/CanvasPanel.h"
#include "HUD_Chess.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_UE52_API UHUD_Chess : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AddMoveWidget(UMove* MoveNotation);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UScrollBox* ScrollBox;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UCanvasPanel* Canvas;

	TArray<UUI_MoveBox*> AllMoves = {};

private:
	UPROPERTY(EditAnywhere, Category = "Child Widgets")
	TSubclassOf<UUserWidget> ChildMoveWidget;

};






