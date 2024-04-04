// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EndNotation.h"

void UUI_EndNotation::NativeConstruct()
{
	Super::NativeConstruct();

	Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextNumber")));
	if (IsValid(Text))
	{
		Text->SetText(FText::FromString("NaN"));
	}
}
