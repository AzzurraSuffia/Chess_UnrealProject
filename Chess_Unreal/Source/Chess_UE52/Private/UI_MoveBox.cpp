// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MoveBox.h"

void UUI_MoveBox::NativeConstruct()
{
	Super::NativeConstruct();

	ExternalBox = Cast<USizeBox>(GetWidgetFromName(TEXT("Box")));
	Button = Cast<UButton>(GetWidgetFromName(TEXT("MoveButton")));
	MoveNotation = Cast<UTextBlock>(GetWidgetFromName(TEXT("MoveString")));
	if (IsValid(ExternalBox) && IsValid(Button) && IsValid(MoveNotation))
	{
		ExternalBox->SetWidthOverride(700.0f);
		ExternalBox->SetHeightOverride(100.0f);
		MoveNotation->SetText(FText::FromString("Move"));
	}
}