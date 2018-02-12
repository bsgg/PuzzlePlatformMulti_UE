// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMMULTI_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer); 
	
	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec) // Console command. Only works in GameInstance
	void Host() override;

	UFUNCTION(Exec) // Console command. Only works in GameInstance
	void Join(const FString& Address) override;
	
	virtual void LoadMainMenu() override;
	

private:
	// Blueprint class
	TSubclassOf<class UUserWidget> MenuClass;	
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;


};
