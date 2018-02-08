// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "../Components/Button.h"
#include "../Components/WidgetSwitcher.h"
#include "../Components/EditableTextBox.h"


bool UMainMenu::Initialize()
{
	bool sucess = Super::Initialize();

	if (!sucess) return false;

	// Join button
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);


	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);	

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);


	if (!ensure(ConfirmJoinMenuButton != nullptr)) return false;
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface)
{
	this->MenuInterface = MenuInterface;
}

void UMainMenu::Setup()
{
	// Add menu to viewport
	this->AddToViewport();

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	playerController->SetInputMode(InputModeData);
	playerController->bShowMouseCursor = true;
	
}

void UMainMenu::Teardown()
{
	this->RemoveFromViewport();

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	playerController->SetInputMode(InputModeData);

	playerController->bShowMouseCursor = false;
}



void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm going to host a server! "));

	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}


void UMainMenu::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm going to join to a server! "));

	if (MenuInterface != nullptr)
	{
		if (!ensure(IPAddressFieldTxt != nullptr)) return;
		const FString Address = IPAddressFieldTxt->GetText().ToString();
		MenuInterface->Join(Address);
	}
}


void UMainMenu::OpenJoinMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm going to Open Join Menu! "));

	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu); 
}


void UMainMenu::OpenMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm going to Open Main menu! "));

	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}


