// Fill out your copyright notice in the Description page of Project Settings.


#include "AIslandGen.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshVoxelFunctions.h"
#include "GeometryScript//MeshNormalsFunctions.h"
#include "GeometryScript//MeshDeformFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAIslandGen::AAIslandGen()
{
}

// Called when the game starts or when spawned
void AAIslandGen::BeginPlay()
{
	Super::BeginPlay();
}

void AAIslandGen::CreateIsland()
{
	// set the seed of the random stream
	RandomStream.Initialize((Seed));
	// Reset the DynamicMesh of the mesh component
	UDynamicMesh* Mesh = this->GetDynamicMeshComponent()->GetDynamicMesh();
	Mesh->Reset();
	// create the island
	this->CreateIslandBase();
}

void AAIslandGen::SetSeed(int32 InputSeed)
{
	// set the seed of the random stream
	RandomStream.Initialize(InputSeed);
}

int32 AAIslandGen::PlatformSwitch(int32 LowLevelOption, int32 HighLevelOption)
{
	FString PlatformName = UGameplayStatics::GetPlatformName();
	if (PlatformName == "IOS" || PlatformName == "Android" || PlatformName == "Switch")
	{
		return LowLevelOption;
	}
	else
	{
		return HighLevelOption;
	}
}

void AAIslandGen::GenerateIslandCones(UDynamicMesh* Mesh)
{
	// generate the cones as the basic shape of the islands
	for(int32 i = 0; i < Islands; i++)
	{
		// generate a random unit vector from the random stream
		FVector RandomVector = RandomStream.GetUnitVector();
		RandomVector *= (MaxSpawnDistance * 0.5f);
		RandomVector.Z = 0.0f;
		
		this->SamplePoints.Add(
			RandomVector);
		
		// make a transform from the random vector
		FVector Translation(
			RandomVector.X,
			RandomVector.Y,
			-800.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FVector Scale(1.0f, 1.0f, 1.0f);
			
		FTransform Transform(Rotation, Translation, Scale);
		
		// generate a radius of a cone island from the random stream
		float Radius = RandomStream.FRandRange(IslandSize.X, IslandSize.Y);
		
		// create a cone mesh for a basic shape of the islands
		FGeometryScriptPrimitiveOptions options;
		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendCone(
			Mesh, options, Transform, Radius, 0.25 * Radius, 1300.0f, 800.0f, 32, true);
	}
}

void AAIslandGen::GenerateBaseForIslands(UDynamicMesh* Mesh) const
{
	// add a box at the base to join the cones
	{
		// make a transform from the random vector
		FVector Translation(
			0.0f,
			0.0f,
			-800.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FVector Scale(1.0f, 1.0f, 1.0f);
			
		FTransform Transform(Rotation, Translation, Scale);

		float DimensionLength = MaxSpawnDistance + 10000.0f;
		// create a cone mesh for a basic shape of the islands
		FGeometryScriptPrimitiveOptions options;
		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(
			Mesh, options, Transform, DimensionLength, DimensionLength, 400.0f);
	}
}

void AAIslandGen::SolidifyAndSmooth(UDynamicMesh* Mesh)
{
	// Solidify the mesh, smooth/tesselate it, and calc normals
	FGeometryScriptSolidifyOptions SolidifyOptions;
	SolidifyOptions.WindingThreshold = 0.5f;
	SolidifyOptions.ExtendBounds = 0.0f;
	SolidifyOptions.SurfaceSearchSteps = 64;
	SolidifyOptions.ShellThickness = 1.0f;

	// the grid parameters for voxelization
	FGeometryScript3DGridParameters GridParameters;
	GridParameters.GridCellSize = 0.25f;

	// check whether the target platform belongs to iOS, Android, or Switch
	int32 GridResolution = PlatformSwitch(60, 50);
	GridParameters.GridResolution = GridResolution;

	UGeometryScriptLibrary_MeshVoxelFunctions::ApplyMeshSolidify(
		Mesh, SolidifyOptions);

	// recompute normals
	FGeometryScriptCalculateNormalsOptions NormalOptions;
	UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals(
		Mesh, NormalOptions);

	// apply mesh smoothing
	FGeometryScriptMeshSelection Selection;
	FGeometryScriptIterativeMeshSmoothingOptions SmoothingOptions;
	SmoothingOptions.NumIterations = 6;
	SmoothingOptions.Alpha = 0.2f;
	UGeometryScriptLibrary_MeshDeformFunctions::ApplyIterativeSmoothingToMesh(
		Mesh, Selection, SmoothingOptions);

	// tesselation
	FGeometryScriptPNTessellateOptions TessellateOptions;
	int32 TessellationLevel = PlatformSwitch(0, 2);
	UGeometryScriptLibrary_MeshSubdivideFunctions::ApplyPNTessellation(
		Mesh, TessellateOptions, TessellationLevel);
}

void AAIslandGen::CreateIslandBase()
{
	UDynamicMesh* Mesh = this->GetDynamicMeshComponent()->GetDynamicMesh();
	// generate the cones as the basic shape of the islands
	GenerateIslandCones(Mesh);
	// generate the base for the islands
	GenerateBaseForIslands(Mesh);
	// solidify, smooth and tessellate the mesh
	SolidifyAndSmooth(Mesh);
}

