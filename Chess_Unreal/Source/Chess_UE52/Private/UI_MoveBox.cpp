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
		Button->OnClicked.AddDynamic(this, &UUI_MoveBox::OnButtonClicked);
	}
}

void UUI_MoveBox::OnButtonClicked()
{

	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->ChessBoard->RestoreSquareColor(GameMode->ChessBoard->TileArray);

	Move->To->SetTileColor(3);
	Move->From->SetTileColor(3);

}