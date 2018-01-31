// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "../Components/Button.h"

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
}


