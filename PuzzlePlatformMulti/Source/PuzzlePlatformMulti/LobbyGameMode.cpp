// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"



void ALobbyGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached 3 Players!"));

		UWorld* world = GetWorld();

		if (!ensure(world != nullptr)) return;
		bUseSeamlessTravel = true;
		world->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
	}
}

void ALobbyGameMode::Logout(AController* Existing)
{
	Super::Logout(Existing);

	--NumberOfPlayers;
}
