// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"


AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// Whether this actor has network authority (server)
	if (HasAuthority())
	{
		FVector loc = GetActorLocation();
		loc += FVector(Speed * deltaTime, 0.0f, 0.0f);
		SetActorLocation(loc);
	}
}

