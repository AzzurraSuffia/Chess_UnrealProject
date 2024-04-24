// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UI_MoveBox.h"
#include "UI_TextBox.h"
#include "UI_ResetButton.h"
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

	//add in the scroll box a move button
	UFUNCTION(BlueprintCallable)
	UUI_MoveBox* AddMoveWidget(UMove* MoveNotation);

	//add in the scroll box a text block 
	UFUNCTION(BlueprintCallable)
	void AddTextWidget(FString& String, FVector2D Position, FVector2D Size);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UScrollBox* ScrollBox;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UCanvasPanel* Canvas;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UCanvasPanel* VisibleZone;

	UUI_ResetButton* ResetButtonWidget;

	//array of all UI_TextBox currently in the scroll box
	TArray<UUI_TextBox*> OtherNotationComponents = {};

	//array of all UI_MoveBox currently in the scroll box
	TArray<UUI_MoveBox*> AllMoves = {};

private:
	UPROPERTY(EditAnywhere, Category = "Child Widgets")
	TSubclassOf<UUserWidget> ChildMoveWidget;

	UPROPERTY(EditAnywhere, Category = "Child Widgets")
	TSubclassOf<UUserWidget> ChildTextWidget;

	UPROPERTY(EditAnywhere, Category = "Child Widgets")
	TSubclassOf<UUserWidget> ResetWidget;
};






