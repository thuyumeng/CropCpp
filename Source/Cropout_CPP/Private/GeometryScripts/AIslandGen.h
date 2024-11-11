// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryScript/MeshVoxelFunctions.h"
#include "Runtime/GeometryFramework/Public/DynamicMeshActor.h"
#include "AIslandGen.generated.h"

UCLASS()
class CROPOUT_CPP_API AAIslandGen : public ADynamicMeshActor
{
	GENERATED_BODY()

	// utility functions to generate the islands
	static int PlatformSwitch(int32 LowLevelOption, int32 HighLevelOption);
	static void SolidifyAndSmooth(UDynamicMesh* Mesh);
	static void CutAndApplyUV(UDynamicMesh* Mesh);
	void GenerateIslandCones(UDynamicMesh* Mesh);
	void GenerateBaseForIslands(UDynamicMesh* Mesh) const;
	
	// use a bunch of cylinders to create the basic shape of the island
	void CreateIslandBase();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Sets default values for this actor's properties
	AAIslandGen();
	// RandomStream used for the island generation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Generation")
	FRandomStream RandomStream;
	// max spawn distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Generation")
	float MaxSpawnDistance = 0.0f;
	// sample points used for the island generation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Generation")
	TArray<FVector> SamplePoints;
	// FVector2D store the maximum and minimum size of the island
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Generation")
	FVector2D IslandSize = FVector2D(500.0f, 8000.0f);
	// the Seed of the random stream
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Generation")
	int32 Seed = 0;
	// the number of islands to generate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Island Generation")
	int32 Islands = 1;
	
	// Create a new island blueprint callable function
	UFUNCTION(BlueprintCallable, Category = "Island Generation")
	void CreateIsland();
	
	// Function to set the seed of the random stream
	UFUNCTION(BlueprintCallable, Category = "Island Generation")
	void SetSeed(int32 InputSeed);
};
