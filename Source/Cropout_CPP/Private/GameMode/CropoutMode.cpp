// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CropoutMode.h"


#include "Chaos/ChaosPerfTest.h"
#include "GameMode/CropGameInstance.h"
#include "Kismet/KismetRenderingLibrary.h"

void ACropoutMode::BeginPlay()
{
	Super::BeginPlay();
	SetupLoadingScreen();
	
}

void ACropoutMode::SetupLoadingScreen() const
{
	// Transition Out the loading screen
	if(UCropGameInstance* GameInstance = Cast<UCropGameInstance>(GetGameInstance()))
	{
		GameInstance->TransitionOut();
	}

	// Clear the render target
	if (GrassRenderTarget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grass Render Target is not set in Cropout Mode."));
	}
	else
	{
		UKismetRenderingLibrary::ClearRenderTarget2D(
			this->GetWorld(), GrassRenderTarget, FLinearColor::Black);
	}
}
