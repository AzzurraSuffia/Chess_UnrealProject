// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI_MoveBox.h"
#include "UI_TextBox.h"
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
	UUI_MoveBox* AddMoveWidget(UMove* MoveNotation);

	UFUNCTION(BlueprintCallable)
	void AddTextWidget(FString& String, FVector2D Position, FVector2D Size);

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UScrollBox* ScrollBox;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UCanvasPanel* Canvas;

	TArray<UUI_TextBox*> OtherNotationComponents = {};
	TArray<UUI_MoveBox*> AllMoves = {};

private:
	UPROPERTY(EditAnywhere, Category = "Child Widgets")
	TSubclassOf<UUserWidget> ChildMoveWidget;

	UPROPERTY(EditAnywhere, Category = "Child Widgets")
	TSubclassOf<UUserWidget> ChildTextWidget;

};






