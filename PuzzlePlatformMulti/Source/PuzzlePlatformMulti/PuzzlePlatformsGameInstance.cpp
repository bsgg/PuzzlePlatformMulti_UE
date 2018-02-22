// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

#include "MenuSystem/MainMenu.h"
#include "PlatformTrigger.h"


const static FName SESSION_NAME = TEXT("My Session Game");

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
		SessionInterface = subSystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{	
			UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance.Init SessionInterface.IsValid "));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);

			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance.Init Starting to find sessions "));

				SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}			
		}		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance We found no subsystem"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance Init Found Class %s"), *MenuClass->GetName());

}

void UPuzzlePlatformsGameInstance::Host() 
{
	UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::Host"));

	if (SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::Host Is valid"));

		auto existingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (existingSession != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::Host Session already exists"));
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnCreateSessionComplete "));

	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnCreateSessionComplete Could not create session"));

		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnCreateSessionComplete %s"), *SessionName.ToString());

	if (Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnCreateSessionComplete Menu not null"));
		Menu->Teardown();
	}

	UEngine* engine = GetEngine();
	
	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Host a new game "));
	
	
	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnCreateSessionComplete Reach end"));

	// Thrown an error 
	world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");    

	UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnCreateSessionComplete Reach end")); 

}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnDestroySessionComplete "));

	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnDestroySessionComplete success Creating session "));
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnFindSessionComplete SUCCESS"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnFindSessionComplete FAIL"));
	}
	
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::CreateSession"));
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettings;
		SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
	}
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


