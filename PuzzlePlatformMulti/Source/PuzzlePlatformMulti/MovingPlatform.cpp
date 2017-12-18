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
}

void AMovingPlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// Whether this actor has network authority (server)
	if (HasAuthority())
	{
		FVector loc = GetActorLocation();
		// Transform target from local to world position
		FVector globalTargetLoc = GetTransform().TransformPosition(TargetLocation);
		FVector Direction = (globalTargetLoc - loc).GetSafeNormal();
		loc += (Speed * deltaTime * Direction);

		
		SetActorLocation(loc);
	}
}
