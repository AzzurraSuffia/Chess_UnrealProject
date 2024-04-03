// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI_MoveBox.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_UE52_API UUI_MoveBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	USizeBox* ExternalBox;

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UButton* Button; 

	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UTextBlock* MoveNotation;

	virtual void NativeConstruct() override;
};
