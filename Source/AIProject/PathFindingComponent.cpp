// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "PathFindingComponent.h"
#include "WaypointComponent.h"
#include "TimeCounterComponent.h"
#include "DrawDebugHelpers.h"
#include "LineTracingForStartAndEnd.h"
#include "GettingStartNode.h"
#include "GettingEndNode.h"
#include "LoadingFromDataMap.h"
#include "FindingPathByAStarAlgorithm.h"
#include "ConvertingResultPath.h"

// Sets default values for this component's properties
UPathFindingComponent::UPathFindingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	PathFindInfo.Timer = CreateDefaultSubobject<UTimeCounterComponent>(FName("TimeCounterComponent")); 

	SetStatus();
	PathFindInfo.OpenList.Empty();
	PathFindInfo.CloseList.Empty();
	PathFindInfo.StartNode = nullptr;
	PathFindInfo.EndNode = nullptr;
	PathFindInfo.StartLocation = FVector::ZeroVector;
	PathFindInfo.EndLocation = FVector::ZeroVector;
	PathFindInfo.RouteData.Empty();
}

// Called when the game starts
void UPathFindingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UPathFindingComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

}

EPathFindingResultState UPathFindingComponent::FindPath(APawn* findPawn, TArray<FVector>& resultRoute, FVector start, FVector end)
{
	if (isFindingPath)
	{
		if (findPawn != findingPawn)
			return EPathFindingResultState::Failed;
	}
	else
	{
		SetStatus(true, findPawn, EPathFindingState::LineTracing);
		PathFindInfo.OpenList.Empty();
		PathFindInfo.CloseList.Empty();
		PathFindInfo.StartNode = nullptr;
		PathFindInfo.EndNode = nullptr;
		PathFindInfo.StartLocation = start;
		PathFindInfo.EndLocation = end;
		ChangePathFindingState(currentState);
	}

	PathFindInfo.Timer->Start(0);
	while (currentState != EPathFindingState::Finished)
	{
		if (PathFindInfo.Timer->GetElapsedTimeFromStart(0) > PathFindInfo.MaxCaluclationTime)
			return EPathFindingResultState::Thinking;
		auto result = pathFinder->FindPath(GetWorld(), PathFindInfo, resultRoute);
		if (result == currentState)
			continue;
		currentState = result;
		ChangePathFindingState(currentState);
		if (result == EPathFindingState::None)
		{
			SetStatus();
			return EPathFindingResultState::Failed;
		}
	}
	SetStatus();
	return EPathFindingResultState::Success;
}

void UPathFindingComponent::DrawPath(FVector start, TArray<FVector>& route, FColor color, float duration, float thickness)
{
	if (duration <= 0.0f)
	{
		DrawDebugLine(GetWorld(), start, route.Last(), color, true, -1.0f, (uint8)'\000', thickness);
		for (auto index = 0; index < route.Num() - 1; index++)
			DrawDebugLine(GetWorld(), route[index], route[index + 1], color, true, -1.0f, (uint8)'\000', thickness);
	}
	else
	{
		DrawDebugLine(GetWorld(), start, route.Last(), color, false, duration, (uint8)'\000', thickness);
		for (auto index = 0; index < route.Num() - 1; index++)
			DrawDebugLine(GetWorld(), route[index], route[index + 1], color, false, duration, (uint8)'\000', thickness);
	}
}

void UPathFindingComponent::SetStatus(bool isFinding, APawn* findPawn, EPathFindingState nextState)
{
	currentState = nextState;
	isFindingPath = isFinding;
	findingPawn = findPawn;
}

void UPathFindingComponent::ChangePathFindingState(EPathFindingState nextState)
{
	switch (nextState)
	{
	case EPathFindingState::None:
		pathFinder = nullptr;
		return;
	case EPathFindingState::LineTracing:
		pathFinder = NewObject<ULineTracingForStartAndEnd>(this, FName("PathFinder_LineTracing"));
		return;
	case EPathFindingState::GettingStartNode:
		pathFinder = NewObject<UGettingStartNode>(this, FName("PathFinder_GettingStartNode"));
		return;
	case EPathFindingState::GettingEndNode:
		pathFinder = NewObject<UGettingEndNode>(this, FName("PathFinder_GettingEndNode"));
		return;
	case EPathFindingState::LoadingFromDataMap:
		pathFinder = NewObject<ULoadingFromDataMap>(this, FName("PathFinder_LoadingFromDataMap"));
		return;
	case EPathFindingState::PathFinding:
		pathFinder = NewObject<UFindingPathByAStarAlgorithm>(this, FName("PathFinder_PathFinding"));
		return;
	case EPathFindingState::ConvertingPathToVector:
		pathFinder = NewObject<UConvertingResultPath>(this, FName("PathFinder_ConvertingResultPath"));
		return;
	case EPathFindingState::Finished:
		pathFinder = nullptr;
		return;
	}
}
