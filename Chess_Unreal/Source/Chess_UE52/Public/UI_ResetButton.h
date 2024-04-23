// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UI_ResetButton.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_UE52_API UUI_ResetButton : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//reference to restart button on viewport
	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UButton* ResetFieldButton;

	virtual void NativeConstruct() override;
};
