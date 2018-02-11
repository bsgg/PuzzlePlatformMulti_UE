// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"

void UMenuWidget::Teardown()
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

void UMenuWidget::Setup()
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

void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterface)
{
	this->MenuInterface = MenuInterface;
}