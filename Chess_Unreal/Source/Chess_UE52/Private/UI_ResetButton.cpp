// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ResetButton.h"

void UUI_ResetButton::NativeConstruct()
{
	Super::NativeConstruct();

	ResetFieldButton = Cast<UButton>(GetWidgetFromName(TEXT("ResetButton")));
}