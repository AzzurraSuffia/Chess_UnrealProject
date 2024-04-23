// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_TextBox.h"

void UUI_TextBox::NativeConstruct()
{
	Super::NativeConstruct();

	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextNotation")));
	if (!IsValid(Text))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast Text Block"));
	}
}
