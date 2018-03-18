// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "OnlineSessionSettings.h"


#include "MenuSystem/MainMenu.h"
#include "PlatformTrigger.h"


const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

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
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);					
		}		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance We found no subsystem"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance Init Found Class %s"), *MenuClass->GetName());

}

void UPuzzlePlatformsGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::Host %s"), *DesiredServerName);

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
	
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnFindSessionComplete Success")); 
		

		/*ServerNames.Add("Test server 1 ");
		ServerNames.Add("Test server 2 ");
		ServerNames.Add("Test server 3 ");*/
		TArray<FServerData> ServerData;
		for (const FOnlineSessionSearchResult& searchResult : SessionSearch->SearchResults)
		{

			UE_LOG(LogTemp, Warning, TEXT("PuzzlePlatformsGameInstance::OnFindSessionComplete Fond session name: %s"), *searchResult.GetSessionIdStr());
			FServerData Data;
			//Data.Name = searchResult.GetSessionIdStr();
			Data.MaxPlayers = searchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers -  searchResult.Session.NumOpenPublicConnections;			
			Data.HostUsername = searchResult.Session.OwningUserName;

			FString serverName;
			if (searchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, serverName))
			{
				Data.Name = serverName;
			}
			else
			{
				Data.Name = "Could not find name.";
			}

			

			ServerData.Add(Data);
		}

		Menu->SetServerList(ServerData);
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
		UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::CreateSession SessionInterface.IsValid()"));

		FOnlineSessionSettings sessionSettings;

		// Check what online subsystem we are using NULL or Steam
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::CreateSession IOnlineSubsystem NULL"));
			sessionSettings.bIsLANMatch = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::CreateSession IOnlineSubsystem STEAM"));
			sessionSettings.bIsLANMatch = false;
		}
		
		sessionSettings.NumPublicConnections = 2;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.bUsesPresence = true;
		sessionSettings.Set(SERVER_NAME_SETTINGS_KEY, FString("Hello"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

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

void UPuzzlePlatformsGameInstance::RefreshingServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::RefreshingServerList() Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		
	}
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

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::Join SessionInterface not valid"));
		return;
	}
	if (!SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::Join SessionSearch not valid"));
		return;
	}
	
	if (Menu != nullptr)
	{
		Menu->Teardown(); 
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);	
	
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
{
	UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::OnJoinSessionComplete")); 

	if (!SessionInterface.IsValid()) return;

	FString address = "";
	
	if (!SessionInterface->GetResolvedConnectString(SessionName, address))
	{
		UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance::OnJoinSessionComplete Unable to get connect string"));
		return;
	}

	UEngine* engine = GetEngine();

	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *address)) ;


	APlayerController* playerContoller = GetFirstLocalPlayerController();

	if (!ensure(playerContoller != nullptr)) return;

	playerContoller->ClientTravel(address, ETravelType::TRAVEL_Absolute);

}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* playerContoller = GetFirstLocalPlayerController();

	if (!ensure(playerContoller != nullptr)) return;

	playerContoller->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}


