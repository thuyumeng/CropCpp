// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CropGameInstance.h"

#include "Blueprint/UserWidget.h"


UUserWidget* UCropGameInstance::GetTransitionUI()
{
	if(UI_Transition.IsNull())
	{
		// load the transition UI
		UI_Transition = CreateWidget(this, UI_TransitionClass.Get());
	}
	return UI_Transition.Get();
}

void UCropGameInstance::CallWidgetFunction(FName FunctionName, void* Parms)
{
	UUserWidget* TransitionUI = GetTransitionUI();
	if (UFunction* Function = TransitionUI->FindFunction(FunctionName))
	{
		TransitionUI->ProcessEvent(Function, Parms);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Function not found: %s"), *FunctionName.ToString());
	}
}

void UCropGameInstance::TransitionIn()
{
	UUserWidget* TransitionUI = GetTransitionUI();
	if(!TransitionUI->IsInViewport())
	{
		TransitionUI->AddToViewport();
	}
	FName FunctionName(TEXT("TransIn"));
	CallWidgetFunction(FunctionName, nullptr);

}

void UCropGameInstance::TransitionOut()
{
	UUserWidget* TransitionUI = GetTransitionUI();
	if(!TransitionUI->IsInViewport())
	{
		TransitionUI->AddToViewport();
	}
	FName FunctionName(TEXT("TransOut"));
	CallWidgetFunction(FunctionName, nullptr);
}
