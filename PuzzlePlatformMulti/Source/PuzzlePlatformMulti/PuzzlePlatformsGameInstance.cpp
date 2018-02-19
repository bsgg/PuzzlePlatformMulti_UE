// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

#include "MenuSystem/MainMenu.h"
#include "PlatformTrigger.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));

	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	IOnlineSubsystem* subSystem = IOnlineSubsystem::Get();

	if (subSystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance.Init subSystem %s found "), *subSystem->GetSubsystemName().ToString());
		IOnlineSessionPtr sessionInterface = subSystem->GetSessionInterface();

		if (sessionInterface.IsValid())
		{
			FOnlineSessionSettings sessionSettings;
			sessionInterface->CreateSession(0,TEXT("My Session Game"), sessionSettings);
			
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance We found no subsystem"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance Init Found Class %s"), *MenuClass->GetName());

}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnCreateSessionComplete %s"), *SessionName.ToString());
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* engine = GetEngine();
	
	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Host a new game "));

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	// Thrown an error 
	world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");   

}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	// Create widget	
	
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	// Add menu to viewport
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);	
	if (!ensure(Menu != nullptr)) return;

	// Add menu to viewport
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	if (Menu != nullptr)
	{
		Menu->Teardown(); 
	}

	UEngine* engine = GetEngine();

	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address)) ;


	APlayerController* playerContoller = GetFirstLocalPlayerController();

	if (!ensure(playerContoller != nullptr)) return;

	playerContoller->ClientTravel(Address, ETravelType::TRAVEL_Absolute); 
	
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* playerContoller = GetFirstLocalPlayerController();

	if (!ensure(playerContoller != nullptr)) return;

	playerContoller->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}


