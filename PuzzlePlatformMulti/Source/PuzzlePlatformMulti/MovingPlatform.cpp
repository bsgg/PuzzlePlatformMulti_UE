// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"


AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable); 
}


void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Set replicates on server and client
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// Whether this actor has network authority (server)
	if (HasAuthority())
	{
		FVector loc = GetActorLocation();

		float journeyLenght = (GlobalTargetLocation - GlobalStartLocation).Size();
		float journeyTravelled = (loc - GlobalStartLocation).Size();

		if (journeyTravelled >= journeyLenght)
		{
			FVector swap = GlobalStartLocation; 
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = swap;
		}
		// Transform target from local to world position
		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		loc += (Speed * deltaTime * Direction); 
		
		SetActorLocation(loc);
	}
}

