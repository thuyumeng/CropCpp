// Fill out your copyright notice in the Description page of Project Settings.


#include "AIslandGen.h"
#include "Kismet/KismetMathLibrary.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"

// Sets default values
AAIslandGen::AAIslandGen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAIslandGen::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAIslandGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAIslandGen::CreateIsland()
{
	// set the seed of the random stream
	this->SetSeed(50);
	// Reset the DynamicMesh of the mesh component
	UDynamicMesh* Mesh = this->GetDynamicMeshComponent()->GetDynamicMesh();
	Mesh->Reset();
	// create the island
	this->CreateIslandBase();
}

void AAIslandGen::SetSeed(int32 Seed)
{
	// set the seed of the random stream
	RandomStream.Initialize(Seed);
}

void AAIslandGen::CreateIslandBase()
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
	Radius = RandomStream.FRandRange(IslandSize.X, IslandSize.Y);

	// create a cone mesh for a basic shape of the islands
	UDynamicMesh* Mesh = this->GetDynamicMeshComponent()->GetDynamicMesh();
	FGeometryScriptPrimitiveOptions options;
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendCone(
		Mesh, options, Transform, Radius, 0.25 * Radius, 1300.0f, 800.0f, 32, true);
}

