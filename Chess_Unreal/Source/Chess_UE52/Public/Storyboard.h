// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "UI_MoveBox.h"
#include "Move.h"
#include "Storyboard.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_UE52_API UStoryboard : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddMoveWidget(UMove* MoveNotation);

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UScrollBox* ScrollBox;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UCanvasPanel* Canvas;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	USizeBox* SizeBox;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UTextBlock* TextBlock;

	TArray<UUI_MoveBox*> AllMoves = {};

	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Child Widgets")
	TSubclassOf<UUserWidget> ChildMoveWidget;
};
