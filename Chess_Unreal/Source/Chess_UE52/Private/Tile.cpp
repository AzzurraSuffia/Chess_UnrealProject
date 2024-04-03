// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Engine/StaticMesh.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	Status = ETileStatus::EMPTY;
	TileGridPosition = FVector2D(0, 0);

}

void ATile::SetTileStatus(const ETileStatus TileStatus)
{
	Status = TileStatus;
}

ETileStatus ATile::GetTileStatus()
{
	return Status;
}

void ATile::SetGridPosition(const double InX, const double InY)
{
	TileGridPosition.Set(InX, InY);
}

FVector2D ATile::GetGridPosition()
{
	return TileGridPosition;
}

void ATile::SetTileColor(int32 color)
{
	if (color == 1)
	{
		StaticMeshComponent->SetMaterial(0, WhiteTile);
	}
	else if (color == 0)
	{
		StaticMeshComponent->SetMaterial(0, BlackTile);
	}
	else if(color == 2)
	{
		StaticMeshComponent->SetMaterial(0, SelectedTile);
	}
	else if(color == 3)
	{
		StaticMeshComponent->SetMaterial(0, SuggestedTile);
	}
	else if (color == 4)
	{
		StaticMeshComponent->SetMaterial(0, CheckTile);
	}
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
//void ATile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}