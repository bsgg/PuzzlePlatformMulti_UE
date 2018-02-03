// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "MenuSystem/MainMenu.h"

#include "PlatformTrigger.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));

	if (!ensure(MenuBPClass.Class != nullptr)) return;

	//UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance Constructor Found class %s"), *platformTriggerBPClass.Class->GetName());

	MenuClass = MenuBPClass.Class;

}

void UPuzzlePlatformsGameInstance::Init()
{

	UE_LOG(LogTemp, Warning, TEXT("UPuzzlePlatformsGameInstance Init Found Class %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* engine = GetEngine();
	
	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap"); 

}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;
	// Create widget	
	
	UMainMenu* menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(menu != nullptr)) return;

	// Add menu to viewport
	menu->AddToViewport(); 

	APlayerController* playerController = GetFirstLocalPlayerController();
	if (!ensure(playerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(menu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	playerController->SetInputMode(InputModeData);
	playerController->bShowMouseCursor = true; 

	menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	UEngine* engine = GetEngine();

	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address)) ;


	APlayerController* playerContoller = GetFirstLocalPlayerController();

	if (!ensure(playerContoller != nullptr)) return;

	playerContoller->ClientTravel(Address, ETravelType::TRAVEL_Absolute); 

	
}


