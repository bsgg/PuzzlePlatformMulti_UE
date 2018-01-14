// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance Constructor"));
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance Init")); 
}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* engine = GetEngine();
	
	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap"); 
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	UEngine* engine = GetEngine();

	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address)) ;


	APlayerController* playerContoller = GetFirstLocalPlayerController();

	if (!ensure(playerContoller != nullptr)) return;

	playerContoller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}


