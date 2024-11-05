// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "RoomDynamicMeshBaseActor.generated.h"

USTRUCT()
struct FRDMBaseInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString id;
	UPROPERTY()
	FString type;
	UPROPERTY()
	FVector Start;
	UPROPERTY()
	FVector End;
	UPROPERTY()
	float Height;
	UPROPERTY()
	float Bottom;
	UPROPERTY()
	float Thickness;
};

class UDynamicMeshComponent;

using UE::Geometry::FDynamicMesh3;

UCLASS()
class ARoomDynamicMeshBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoomDynamicMeshBaseActor();
	
	UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UDynamicMeshComponent> DynamicMeshComponent;

	void InitializeMesh(const TArray<FVector2D>& Outline,float Height,const FString& MaterialPath);

private:
	void CreateMeshWithHole(UDynamicMeshComponent* DynamicMeshComponent, const FRDMBaseInfo& WallInfo, const TArray<FRDMBaseInfo>& HoleInfos);
	
	// Height,Thickness must be in [cm]
	FDynamicMesh3 CreateMesh(const FVector& Start,const FVector& End,float Height, float Thickness);

	FRDMBaseInfo BaseInfo;
};

