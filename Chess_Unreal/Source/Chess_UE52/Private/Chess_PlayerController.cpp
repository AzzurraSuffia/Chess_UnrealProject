// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_PlayerController.h"
#include "Components/InputComponent.h"
#include "Components/CanvasPanelSlot.h"

AChess_PlayerController::AChess_PlayerController()
{ 
	//non cattura il mouse quando faccio play
	bShowMouseCursor = true;
	//abilito evento click
	bEnableClickEvents = true;
}

void AChess_PlayerController::ClickOnGrid()
{
	const auto HumanPlayer = Cast<AChess_HumanPlayer>(GetPawn());
	if (IsValid(HumanPlayer))
	{
		HumanPlayer->OnClick();
	}

}

void AChess_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ChessContext, 0);
	}

	if (HUDWidget)
	{
		UUserWidget* HUDChessUserWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidget);
		if (HUDChessUserWidget)
		{
			HUDChess = Cast<UHUD_Chess>(HUDChessUserWidget);
			if (IsValid(HUDChess))
			{
				HUDChess->AddToViewport();
				GameMode->OnStart.Broadcast();
			}
		}
	};
}

void AChess_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AChess_PlayerController::ClickOnGrid);
	}

}
