// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "nlohmann/json.hpp"
#include "RoomForgeSubsystem.generated.h"

using json = nlohmann::json;
/**
 * 
 */
UCLASS()
class ROOMFORGE_API URoomForgeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BuildSceneFromJson(const FString& JsonPath,UWorld* World, const FString& MaterialPath);

private:
	json LoadJson(const FString& JsonPath);
};
