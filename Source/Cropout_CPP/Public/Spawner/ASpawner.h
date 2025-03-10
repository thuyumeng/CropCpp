// The actor used for spawning Biomes and workers?

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASpawner.generated.h"

UCLASS()
class CROPOUT_CPP_API AASpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
