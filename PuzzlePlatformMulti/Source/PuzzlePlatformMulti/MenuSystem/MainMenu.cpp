// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"

#include "../Components/Button.h"
#include "../Components/WidgetSwitcher.h"
#include "../Components/EditableTextBox.h"
#include "../Components/TextBlock.h"

#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> serverRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));

	if (!ensure(serverRowBPClass.Class != nullptr)) return;
	ServerRowClass = serverRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool sucess = Super::Initialize();

	if (!sucess) return false;

	// Join button
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);	

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConfirmJoinMenuButton != nullptr)) return false;
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}


void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm going to host a server! "));

	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	UWorld* world = this->GetWorld();
	if (!ensure(world != nullptr)) return;

	ServerList->ClearChildren();

	for (const FString& serverName : ServerNames)
	{
		UServerRow* row = CreateWidget<UServerRow>(world, ServerRowClass);
		if (!ensure(row != nullptr)) return;

		row->ServerName->SetText(FText::FromString(serverName));

		ServerList->AddChild(row);
	}
}


void UMainMenu::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm going to join to a server! "));

	if (MenuInterface != nullptr)
	{
		/*if (!ensure(IPAddressFieldTxt != nullptr)) return;
		const FString Address = IPAddressFieldTxt->GetText().ToString();*/

		MenuInterface->Join("");	

	}
}

void UMainMenu::QuitPressed()
{
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;

	playerController->ConsoleCommand("quit");  
}


void UMainMenu::OpenJoinMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm going to Open Join Menu! "));

	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu); 

	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshingServerList();
	}
}


void UMainMenu::OpenMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm going to Open Main menu! "));

	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}


