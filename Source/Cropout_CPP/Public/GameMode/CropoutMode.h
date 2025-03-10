// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CropoutMode.generated.h"

/**
 * 
 */
UCLASS()
class CROPOUT_CPP_API ACropoutMode : public AGameModeBase
{
	GENERATED_BODY()

	// Event BeginPlay
	virtual void BeginPlay() override;
	// Set loading screen to animate out, reset render target.
	void SetupLoadingScreen() const;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Render")
	UTextureRenderTarget2D* GrassRenderTarget;
};
