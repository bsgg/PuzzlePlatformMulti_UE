// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"



void ALobbyGameMode::PostLogin(APlayerController* newPlayer)
{
	++NumberOfPlayers;

	if (NumberOfPlayers >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reached 3 Players!"));
	}
}

void ALobbyGameMode::Logout(AController* Existing)
{
	--NumberOfPlayers;
}
