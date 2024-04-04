// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UI_EndNotation.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_UE52_API UUI_EndNotation : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite,/* meta = (BindWidget),*/ Category = "Widgets Components")
	UTextBlock* Text;

	virtual void NativeConstruct() override;
};
