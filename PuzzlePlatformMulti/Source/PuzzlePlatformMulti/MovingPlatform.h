// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMMULTI_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()	
	
public:
	AMovingPlatform();

	UPROPERTY(EditAnywhere, Category = "Settings")
	float Speed = 20.0f;

protected:
	virtual void Tick(float deltaTime) override;
	
	
};
