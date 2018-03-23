// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformMultiGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMMULTI_API ALobbyGameMode : public APuzzlePlatformMultiGameMode
{
	GENERATED_BODY()

public:

	 void PostLogin(APlayerController* newPlayer) override;

	 void Logout(AController* Existing) override;
	
private:

	uint32 NumberOfPlayers = 0;
};
