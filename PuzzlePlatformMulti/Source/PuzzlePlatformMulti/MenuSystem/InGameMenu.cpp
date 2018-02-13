// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"

#include "../Components/Button.h"


bool UInGameMenu::Initialize()
{
	bool sucess = Super::Initialize();

	if (!sucess) return false;

	// Buttons
	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPress);


	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPress);


	return true;

}

void UInGameMenu::CancelPress()
{ 
	Teardown();
}


void UInGameMenu::QuitPress()
{

	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}
