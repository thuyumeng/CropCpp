// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeometryHelper.generated.h"

/**
 * 
 */
UCLASS()
class CROPOUT_CPP_API UGeometryHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Geometry Script")
	static USplineComponent* CreateOffsetedSpline(USplineComponent* Dest, const USplineComponent* Source, FVector Offset);
};
