// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomForgeSubsystem.h"
#include "RoomDynamicMeshBaseActor.h"
#include <fstream>

#include "Components/DynamicMeshComponent.h"

void URoomForgeSubsystem::BuildSceneFromJson(const FString& JsonPath, UWorld* World, const FString& MaterialPath)
{
	json JsonObject = LoadJson(JsonPath);
	if(JsonObject.is_null()) return;
	
	for(const auto& wall : JsonObject["walls"])
	{
		if(wall.is_null()) continue;
		FRDMBaseInfo WallInfo;
		WallInfo.id = wall["id"].get<std::string>().c_str();
		WallInfo.type = wall["type"].get<std::string>().c_str();
		WallInfo.Start = FVector(wall["start"]["x"].get<float>() * 100,wall["start"]["y"].get<float>() * 100,0.f);
		WallInfo.End = FVector(wall["end"]["x"].get<float>() * 100,wall["end"]["y"].get<float>() * 100,0.f);
		WallInfo.Height = wall["height"].get<float>() * 100;
		WallInfo.Thickness = wall["thickness"].get<float>() * 100;

		TArray<FRDMBaseInfo> HoleInfos;
		for(const auto& hole:wall["holeInfos"])
		{
			if(hole.is_null()) continue;
			FRDMBaseInfo HoleInfo;
			HoleInfo.Start = FVector(hole["start"]["x"].get<float>() * 100,hole["start"]["y"].get<float>() * 100,0.f);
			HoleInfo.End = FVector(hole["end"]["x"].get<float>() * 100,hole["end"]["y"].get<float>() * 100,0.f);
			HoleInfo.Height = hole["height"].get<float>() * 100;
			HoleInfo.Bottom = hole["bottom"].get<float>() * 100;
			HoleInfos.Add(HoleInfo);
		}

		ARoomDynamicMeshBaseActor* DMA = Cast<ARoomDynamicMeshBaseActor>(World->SpawnActor(ARoomDynamicMeshBaseActor::StaticClass()));
		if(!DMA) return;
		DMA->CreateMeshWithHole(WallInfo,HoleInfos);

		UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *MaterialPath));
		if(Material)
		{
			DMA->DynamicMeshComponent->SetMaterial(0,Material);
		}
	}
}

json URoomForgeSubsystem::LoadJson(const FString& JsonPath)
{
	std::ifstream File(TCHAR_TO_UTF8(*JsonPath));
	if(!File.is_open())
	{
		UE_LOG(LogTemp,Warning,TEXT("Failed to open JSON file : %s"), *JsonPath);
		return nullptr;
	}

	json JosnObject;
	File >> JosnObject;
	File.close();
	return JosnObject;
}
