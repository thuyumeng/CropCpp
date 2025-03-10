// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CropGameInstance.generated.h"

/**
 *  the game instance of the Cropout game
 */
UCLASS()
class CROPOUT_CPP_API UCropGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	// The transition UI object
	TSoftObjectPtr<UUserWidget> UI_Transition;
	// Get the transition UI object can not be const
	// because the UI_Transition is not const has to be loaded
	inline UUserWidget* GetTransitionUI();
	// Call widget's function
	inline void CallWidgetFunction(FName FunctionName, void* Parms);
public:
	// The transition UI class blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> UI_TransitionClass;
	
	// transition in ui animation
	UFUNCTION(BlueprintCallable, Category="UI")
	void TransitionIn();
	UFUNCTION(BlueprintCallable, Category="UI")
	void TransitionOut();
};
