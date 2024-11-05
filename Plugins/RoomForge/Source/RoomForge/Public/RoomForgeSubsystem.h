// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RoomForgeSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ROOMFORGE_API URoomForgeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void BuildSceneFromJson(const FString& JsonPath,UWorld* World);
	
};
