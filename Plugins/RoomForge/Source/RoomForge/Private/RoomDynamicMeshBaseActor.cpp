// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomDynamicMeshBaseActor.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMesh/MeshNormals.h"
#include "Generators/RectangleMeshGenerator.h"
#include "Operations/MeshBoolean.h"

ARoomDynamicMeshBaseActor::ARoomDynamicMeshBaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	DynamicMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>("DynamicMeshComponent");
	RootComponent = DynamicMeshComponent;
}

void ARoomDynamicMeshBaseActor::InitializeMesh(const TArray<FVector2D>& Outline,float Height,const FString& MaterialPath)
{
	
}

void ARoomDynamicMeshBaseActor::CreateMeshWithHole(const FRDMBaseInfo& WallInfo, const TArray<FRDMBaseInfo>& HoleInfos)
{
	if(!DynamicMeshComponent) return;

	// 创建主墙体
	FDynamicMesh3 WallMesh = CreateMesh(WallInfo.Start,WallInfo.End,WallInfo.Height,WallInfo.Thickness);

	// 对每个孔洞进行bool运算
	for(int32 i = 0; i < HoleInfos.Num(); i++)
	{
		FVector HoleStart = HoleInfos[i].Start;
		FVector HoleEnd = HoleInfos[i].End;
		float HoleHeight = HoleInfos[i].Height;
		float HoleBottom = HoleInfos[i].Bottom;

		FDynamicMesh3 HolMesh = CreateMesh(HoleInfos[i].Start,HoleInfos[i].End,HoleInfos[i].Height,HoleInfos[i].Bottom);

		UE::Geometry::FMeshBoolean MeshBoolean(&WallMesh,&HolMesh,&WallMesh,UE::Geometry::FMeshBoolean::EBooleanOp::Difference);
		MeshBoolean.Compute();

		HolMesh.Clear();
	}

	DynamicMeshComponent->GetDynamicMesh()->GetMeshRef() = WallMesh;
	UE::Geometry::FMeshNormals Normals(&WallMesh);
	Normals.ComputeVertexNormals();

	DynamicMeshComponent->NotifyMeshUpdated();
}

// TODO 确认start->end是中轴线还是靠右或靠左，目前按靠右挤出
FDynamicMesh3 ARoomDynamicMeshBaseActor::CreateMesh(const FVector& Start, const FVector& End, float Height, float Thickness)
{
	FDynamicMesh3 NewMesh;

	FVector WallDirection = (End - Start).GetSafeNormal();
	FVector UpVector = FVector::UpVector;
	FVector RightVector = FVector::CrossProduct(UpVector, WallDirection).GetSafeNormal() * Thickness;
	
	int32 V0 = NewMesh.AppendVertex(Start);
	int32 V1 = NewMesh.AppendVertex(Start + RightVector);
	int32 V2 = NewMesh.AppendVertex(End + RightVector);
	int32 V3 = NewMesh.AppendVertex(End);
	int32 V4 = NewMesh.AppendVertex(Start + FVector(0,0,Height));
	int32 V5 = NewMesh.AppendVertex((Start + RightVector) + FVector(0,0,Height));
	int32 V6 = NewMesh.AppendVertex((End + RightVector) + FVector(0,0,Height));
	int32 V7 = NewMesh.AppendVertex(End + FVector(0,0,Height));
	
	NewMesh.AppendTriangle(V0, V1, V2);
	NewMesh.AppendTriangle(V0, V2, V3);
	NewMesh.AppendTriangle(V4, V5, V6);
	NewMesh.AppendTriangle(V4, V6, V7);
	NewMesh.AppendTriangle(V0, V1, V5);
	NewMesh.AppendTriangle(V0, V5, V4);
	NewMesh.AppendTriangle(V1, V2, V6);
	NewMesh.AppendTriangle(V1, V6, V5);
	NewMesh.AppendTriangle(V2, V3, V7);
	NewMesh.AppendTriangle(V2, V7, V6);
	NewMesh.AppendTriangle(V3, V0, V4);
	NewMesh.AppendTriangle(V3, V4, V7);

	return NewMesh;
}




