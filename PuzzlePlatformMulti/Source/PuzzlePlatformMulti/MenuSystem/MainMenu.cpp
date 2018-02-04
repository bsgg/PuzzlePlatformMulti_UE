// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "../Components/Button.h"


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

bool UMainMenu::Initialize()
{
	bool sucess = Super::Initialize();

	if (!sucess) return false;

	// Setup
	if (!ensure(HostButton != nullptr)) return false;

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer); 

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


