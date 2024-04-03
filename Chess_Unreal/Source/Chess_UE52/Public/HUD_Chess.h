// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Storyboard.h"
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
	UPROPERTY(EditAnywhere, Category = "Child Widgets")
	TSubclassOf<UUserWidget> StoryboardWidgetClass;

	UCanvasPanel* CanvasHUD;

	UStoryboard* Storyboard;

	virtual void NativeConstruct() override;
};
