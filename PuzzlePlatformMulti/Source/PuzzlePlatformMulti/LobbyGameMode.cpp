// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "TimerManager.h"

#include "PuzzlePlatformsGameInstance.h"


void ALobbyGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached 3 Players!"));

		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10);
	}
}

void ALobbyGameMode::Logout(AController* Existing)
{
	Super::Logout(Existing);

	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame() 
{
	UPuzzlePlatformsGameInstance * gameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());

	if (gameInstance == nullptr) return;
	
	gameInstance->StartSession();

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	bUseSeamlessTravel = true;
	world->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}
