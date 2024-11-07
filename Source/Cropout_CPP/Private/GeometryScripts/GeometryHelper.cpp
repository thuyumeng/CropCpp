// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout_CPP/Public/GeometryScripts/GeometryHelper.h"

static FSplinePoint ExtractPointAtIndex(const USplineComponent* Spline, int32 Index)
{
	const ESplineCoordinateSpace::Type C = ESplineCoordinateSpace::Local;
	return FSplinePoint{
		0.0f,
		Spline->GetLocationAtSplinePoint(Index, C),
		Spline->GetArriveTangentAtSplinePoint(Index, C),
		Spline->GetLeaveTangentAtSplinePoint(Index, C),
		Spline->GetRotationAtSplinePoint(Index, C),
		Spline->GetScaleAtSplinePoint(Index)
	};
}

static FSplinePoint ExtractPointAtDistanceAlongSpline(const USplineComponent* Spline, float Distance)
{
	const ESplineCoordinateSpace::Type C = ESplineCoordinateSpace::Local;
	const FVector Tangent = Spline->GetTangentAtDistanceAlongSpline(Distance, C);
	return FSplinePoint{
		0.0f,
		Spline->GetLocationAtDistanceAlongSpline(Distance, C),
		Tangent,
		Tangent,
		Spline->GetRotationAtDistanceAlongSpline(Distance, C),
		Spline->GetScaleAtDistanceAlongSpline(Distance)
	};
}

USplineComponent* UGeometryHelper::CreateOffsetedSpline(USplineComponent* Dest, const USplineComponent* Source, FVector Offset)
{
	if (!IsValid(Dest))
		return Dest;

	ON_SCOPE_EXIT{ Dest->UpdateSpline();};
	// Clear the existing points
	Dest->ClearSplinePoints(false);
	
	if (!IsValid(Source))
		return Dest;
	
	const int32 Count = Source->GetNumberOfSplinePoints();
	if (Count == 0)
		return Dest;

	// Add the points
	auto AddPoint = [Dest, Key = 0]  (FSplinePoint Point) mutable->void
	{
		Point.InputKey = static_cast<float>(Key++);
		Dest->AddPoint(Point, false);
	};

	const float CornerRadius = 10.f;

	// the start bar
	{
		// the points on the floor
		FSplinePoint Floor = ExtractPointAtIndex(Source, 0);
		Floor.Rotation = Offset.Rotation();
		Floor.ArriveTangent = Floor.LeaveTangent = Offset;
		
		FSplinePoint LoopCut = Floor;
		const float OffsetMagnitude = Offset.Length();
		const float LoopCutMagnitude = OffsetMagnitude - CornerRadius;
		const FVector LoopCutOffset = Offset * (LoopCutMagnitude / OffsetMagnitude);
		LoopCut.Position += LoopCutOffset;
		LoopCut.LeaveTangent *= (2.0f * CornerRadius / LoopCut.LeaveTangent.Length());

		FSplinePoint CurveStart = ExtractPointAtDistanceAlongSpline(Source, CornerRadius);
		CurveStart.Position += Offset;
		CurveStart.ArriveTangent *= (2.0f * CornerRadius / CurveStart.ArriveTangent.Length());

		AddPoint(Floor);
		AddPoint(LoopCut);
		AddPoint(CurveStart);
	}

	// eliminate the first point and split it into two points for the corner
	for (int32 i = 1; i < Count - 1; ++i)
	{
		FSplinePoint Point = ExtractPointAtIndex(Source, i);
		Point.Position += Offset;
		AddPoint(Point);
	}
	
	// the end bar
	{
		// the points on the floor
		FSplinePoint Floor = ExtractPointAtIndex(Source, Count - 1);
		Floor.Rotation = (-Offset).Rotation();
		Floor.ArriveTangent = Floor.LeaveTangent = -Offset;

		FSplinePoint LoopCut = Floor;
		const float OffsetMagnitude = Offset.Length();
		const float LoopCutMagnitude = OffsetMagnitude - CornerRadius;
		const FVector LoopCutOffset = Offset * (LoopCutMagnitude / OffsetMagnitude);
		LoopCut.Position += LoopCutOffset;
		LoopCut.ArriveTangent *= (2.0f * CornerRadius / LoopCut.ArriveTangent.Length());

		float distance = Source->GetDistanceAlongSplineAtSplinePoint(Count - 1) - CornerRadius;
		FSplinePoint CurveEnd = ExtractPointAtDistanceAlongSpline(Source, distance);
		CurveEnd.Position += Offset;
		CurveEnd.LeaveTangent *= (2.0f * CornerRadius / CurveEnd.LeaveTangent.Length());

		AddPoint(CurveEnd);
		AddPoint(LoopCut);
		AddPoint(Floor);
	}
	
	return Dest;
}
