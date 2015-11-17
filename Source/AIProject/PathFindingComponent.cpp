// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "PathFindingComponent.h"
#include "WaypointComponent.h"
#include "TimeCounterComponent.h"
#include "DrawDebugHelpers.h"

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
	PathFindInfo.FindingIndex = 0;

	Waypoints.Empty();
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
		SetStatus(true, findPawn);
		PathFindInfo.OpenList.Empty();
		PathFindInfo.CloseList.Empty();
		PathFindInfo.StartNode = nullptr;
		PathFindInfo.EndNode = nullptr;
		PathFindInfo.StartLocation = start;
		PathFindInfo.EndLocation = end;
		PathFindInfo.FindingIndex = 0;

	}

	switch (currentState)
	{
	case EPathFindingState::LineTracing:
		switch (LineTraceForTwoLocations(resultRoute))
		{
		case EPathFindingResultState::Success:	SetStatus();																return EPathFindingResultState::Success;
		case EPathFindingResultState::Thinking:	SetStatus(true, findingPawn, EPathFindingState::GettingStartNode);			return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Failed:	SetStatus();																return EPathFindingResultState::Failed;
		}
	case EPathFindingState::GettingStartNode:
		switch (GetStartNode())
		{
		case EPathFindingResultState::Success:	SetStatus(true, findingPawn, EPathFindingState::GettingEndNode);			return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Thinking:	SetStatus(true, findingPawn, EPathFindingState::GettingStartNode);			return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Failed:	SetStatus();																return EPathFindingResultState::Failed;
		}
	case EPathFindingState::GettingEndNode:
		switch (GetEndNode())
		{
		case EPathFindingResultState::Success:	SetStatus(true, findingPawn, EPathFindingState::PathFinding);				return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Thinking:	SetStatus(true, findingPawn, EPathFindingState::GettingEndNode);			return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Failed:	SetStatus();																return EPathFindingResultState::Failed;
		}
	case EPathFindingState::PathFinding:
		switch (FindPathByAStarAlgorithm())
		{
		case EPathFindingResultState::Success:	SetStatus(true, findingPawn, EPathFindingState::ConvertingPathToVector);	return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Thinking:	SetStatus(true, findingPawn, EPathFindingState::PathFinding);				return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Failed:	SetStatus();																return EPathFindingResultState::Failed;
		}
	case EPathFindingState::ConvertingPathToVector:
		switch (ConvertRouteToVector(resultRoute))
		{
		case EPathFindingResultState::Success:	SetStatus();																return EPathFindingResultState::Success;
		case EPathFindingResultState::Thinking:	SetStatus(true, findingPawn, EPathFindingState::ConvertingPathToVector);	return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Failed:	SetStatus();																return EPathFindingResultState::Failed;
		}
	}

	SetStatus();
	return EPathFindingResultState::Failed;
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

EPathFindingResultState UPathFindingComponent::LineTraceForTwoLocations(TArray<FVector>& resultRoute)
{
	if (GetWorld()->LineTraceTestByObjectType(PathFindInfo.StartLocation, PathFindInfo.EndLocation, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)))
		return EPathFindingResultState::Thinking;
	resultRoute.Empty();
	resultRoute.Add(PathFindInfo.EndLocation);
	return EPathFindingResultState::Success;
}

EPathFindingResultState UPathFindingComponent::GetStartNode()
{
	if (Waypoints.Num() == 0)
		return EPathFindingResultState::Failed;
	static auto minDistance = 0.0f;
	PathFindInfo.Timer->Start(0);
	if (PathFindInfo.FindingIndex == 0)
	{
		PathFindInfo.StartNode = Waypoints[0];
		minDistance = (PathFindInfo.StartNode->GetComponentLocation() - PathFindInfo.StartLocation).Size();
		PathFindInfo.FindingIndex = 1;
	}
	while (PathFindInfo.FindingIndex < Waypoints.Num())
	{
		auto currentNode = Waypoints[PathFindInfo.FindingIndex];
		auto currentDistance = (currentNode->GetComponentLocation() - PathFindInfo.StartLocation).Size();
		if (currentDistance < minDistance)
		{
			PathFindInfo.StartNode = currentNode;
			minDistance = currentDistance;
		}
		PathFindInfo.FindingIndex++;

		if (PathFindInfo.Timer->GetElapsedTimeFromStart(0) >= MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}
	PathFindInfo.FindingIndex = 0;
	return EPathFindingResultState::Success;
}

EPathFindingResultState UPathFindingComponent::GetEndNode()
{
	if (Waypoints.Num() == 0)
		return EPathFindingResultState::Failed;
	static auto minDistance = 0.0f;
	PathFindInfo.Timer->Start(0);
	if (PathFindInfo.FindingIndex == 0)
	{
		PathFindInfo.EndNode = Waypoints[0];
		minDistance = (PathFindInfo.EndNode->GetComponentLocation() - PathFindInfo.EndLocation).Size();
		PathFindInfo.FindingIndex = 1;
	}
	while (PathFindInfo.FindingIndex < Waypoints.Num())
	{
		auto currentNode = Waypoints[PathFindInfo.FindingIndex];
		auto currentDistance = (currentNode->GetComponentLocation() - PathFindInfo.EndLocation).Size();
		if (currentDistance < minDistance)
		{
			PathFindInfo.EndNode = currentNode;
			minDistance = currentDistance;
		}
		PathFindInfo.FindingIndex++;

		if (PathFindInfo.Timer->GetElapsedTimeFromStart(0) >= MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}
	PathFindInfo.FindingIndex = 0;
	return EPathFindingResultState::Success;
}

EPathFindingResultState UPathFindingComponent::FindPathByAStarAlgorithm()
{
	PathFindInfo.Timer->Start(0);
	if (PathFindInfo.FindingIndex == 0)
		PathFindInfo.OpenList.Add(PathFindInfo.StartNode);
	while (PathFindInfo.OpenList.Num() > 0)
	{
		auto currentNode = GetMinCostNode(PathFindInfo.OpenList);
		currentNode->Cost = 0.0f;
		currentNode->State = EState::Closed;
		PathFindInfo.CloseList.Add(currentNode);
		PathFindInfo.OpenList.Remove(currentNode);
		if (currentNode->ID == PathFindInfo.EndNode->ID)
		{
			PathFindInfo.FindingIndex = 0;
			return EPathFindingResultState::Success;
		}
		for (auto neighbor : currentNode->NeighborWaypoints)
		{
			if (PathFindInfo.CloseList.Contains(neighbor))
				continue;
			auto startToCurrent = (PathFindInfo.StartNode->GetComponentLocation() - currentNode->GetComponentLocation()).Size();
			auto endToNeighbor = (PathFindInfo.EndNode->GetComponentLocation() - neighbor->GetComponentLocation()).Size();
			auto neighborToCurrent = (neighbor->GetComponentLocation() - currentNode->GetComponentLocation()).Size();
			auto cost = startToCurrent + endToNeighbor + neighborToCurrent;
			if (cost < neighbor->Cost || neighbor->Cost == 0.0f)
			{
				neighbor->Cost = cost;
				neighbor->ParentWaypoint = currentNode;
			}
			if (!PathFindInfo.OpenList.Contains(neighbor))
			{
				neighbor->State = EState::Open;
				PathFindInfo.OpenList.Add(neighbor);
			}
		}
		PathFindInfo.FindingIndex++;
		if (PathFindInfo.Timer->GetElapsedTimeFromStart(0) > MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}

	PathFindInfo.FindingIndex = 0;
	return EPathFindingResultState::Failed;
}

EPathFindingResultState UPathFindingComponent::ConvertRouteToVector(TArray<FVector>& route)
{
	static auto currentNode = PathFindInfo.EndNode;
	PathFindInfo.Timer->Start(0);
	if (PathFindInfo.FindingIndex == 0)
	{
		route.Add(PathFindInfo.EndLocation);
		route.Add(PathFindInfo.EndNode->GetComponentLocation());
		currentNode = PathFindInfo.EndNode;
	}
	while (currentNode->ID != PathFindInfo.StartNode->ID)
	{
		currentNode = currentNode->ParentWaypoint;
		route.Add(currentNode->GetComponentLocation());
		PathFindInfo.FindingIndex++;
		if (PathFindInfo.Timer->GetElapsedTimeFromStart(0) > MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}
	for (auto node : PathFindInfo.OpenList)
	{
		node->State = EState::None;
		node->Cost = 0.0f;
		node->ParentWaypoint = nullptr;
	}
	for (auto node : PathFindInfo.CloseList)
	{
		node->State = EState::None;
		node->Cost = 0.0f;
		node->ParentWaypoint = nullptr;
	}

	PathFindInfo.FindingIndex = 0;
	return EPathFindingResultState::Success;
}

UWaypointComponent* UPathFindingComponent::GetMinCostNode(TArray<UWaypointComponent*>& waypointList)
{
	auto currentWaypoint = waypointList[0];
	auto minCost = currentWaypoint->Cost;
	for (auto index = 1; index < waypointList.Num(); index++)
	{
		if (waypointList[index]->Cost < minCost)
		{
			currentWaypoint = waypointList[index];
			minCost = currentWaypoint->Cost;
		}
	}

	return currentWaypoint;
}
