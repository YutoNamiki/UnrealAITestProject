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

	timer = CreateDefaultSubobject<UTimeCounterComponent>(FName("TimeCounterComponent")); 

	SetStatus();
	openList.Empty();
	closeList.Empty();
	startNode = nullptr;
	endNode = nullptr;
	startLocation = FVector::ZeroVector;
	endLocation = FVector::ZeroVector;
	findingIndex = 0;

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
		openList.Empty();
		closeList.Empty();
		startNode = nullptr;
		endNode = nullptr;
		startLocation = start;
		endLocation = end;
		findingIndex = 0;
	}

	switch (currentState)
	{
	case EPathFindingState::LineTracing:
		switch (LineTraceForTwoLocations(startLocation, endLocation, resultRoute))
		{
		case EPathFindingResultState::Success:	SetStatus();																return EPathFindingResultState::Success;
		case EPathFindingResultState::Thinking:	SetStatus(true, findingPawn, EPathFindingState::GettingStartNode);			return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Failed:	SetStatus();																return EPathFindingResultState::Failed;
		}
	case EPathFindingState::GettingStartNode:
		switch (GetClosetNode(Waypoints, startLocation, startNode))
		{
		case EPathFindingResultState::Success:	SetStatus(true, findingPawn, EPathFindingState::GettingEndNode);			return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Thinking:	SetStatus(true, findingPawn, EPathFindingState::GettingStartNode);			return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Failed:	SetStatus();																return EPathFindingResultState::Failed;
		}
	case EPathFindingState::GettingEndNode:
		switch (GetClosetNode(Waypoints, endLocation, endNode))
		{
		case EPathFindingResultState::Success:	SetStatus(true, findingPawn, EPathFindingState::PathFinding);				return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Thinking:	SetStatus(true, findingPawn, EPathFindingState::GettingEndNode);			return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Failed:	SetStatus();																return EPathFindingResultState::Failed;
		}
	case EPathFindingState::PathFinding:
		switch (FindPathByAStarAlgorithm(startNode, endNode, openList, closeList))
		{
		case EPathFindingResultState::Success:	SetStatus(true, findingPawn, EPathFindingState::ConvertingPathToVector);	return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Thinking:	SetStatus(true, findingPawn, EPathFindingState::PathFinding);				return EPathFindingResultState::Thinking;
		case EPathFindingResultState::Failed:	SetStatus();																return EPathFindingResultState::Failed;
		}
	case EPathFindingState::ConvertingPathToVector:
		switch (ConvertRouteToVector(startNode, endNode, endLocation, openList, closeList, resultRoute))
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
		for (auto index = 0; index < route.Num() - 2; index++)
			DrawDebugLine(GetWorld(), route[index], route[index + 1], color, true, -1.0f, (uint8)'\000', thickness);
	}
	else
	{
		DrawDebugLine(GetWorld(), start, route.Last(), color, false, duration, (uint8)'\000', thickness);
		for (auto index = 0; index < route.Num() - 2; index++)
			DrawDebugLine(GetWorld(), route[index], route[index + 1], color, false, duration, (uint8)'\000', thickness);
	}
}

void UPathFindingComponent::SetStatus(bool isFinding, APawn* findPawn, EPathFindingState nextState)
{
	currentState = nextState;
	isFindingPath = isFinding;
	findingPawn = findPawn;
}

EPathFindingResultState UPathFindingComponent::LineTraceForTwoLocations(FVector start, FVector end, TArray<FVector>& resultRoute)
{
	if (GetWorld()->LineTraceTestByObjectType(start, end, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)))
		return EPathFindingResultState::Thinking;
	resultRoute.Empty();
	resultRoute.Add(end);
	return EPathFindingResultState::Success;
}

EPathFindingResultState UPathFindingComponent::GetClosetNode(TArray<UWaypointComponent*>& waypointList, FVector location, UWaypointComponent* closetNode)
{
	if (waypointList.Num() == 0)
		return EPathFindingResultState::Failed;
	static auto minDistance = 0.0f;
	timer->Start(0);
	if (findingIndex == 0)
	{
		closetNode = waypointList[0];
		minDistance = (closetNode->GetComponentLocation() - location).Size();
		findingIndex = 1;
	}
	while (findingIndex < waypointList.Num())
	{
		auto currentNode = waypointList[findingIndex];
		auto currentDistance = (currentNode->GetComponentLocation() - location).Size();
		if (currentDistance < minDistance)
		{
			closetNode = currentNode;
			minDistance = currentDistance;
		}
		findingIndex++;

		if (timer->GetElapsedTimeFromStart(0) >= MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}
	findingIndex = 0;
	return EPathFindingResultState::Success;
}

EPathFindingResultState UPathFindingComponent::FindPathByAStarAlgorithm(UWaypointComponent* start, UWaypointComponent* end, TArray<UWaypointComponent*>& open, TArray<UWaypointComponent*>& close)
{
	timer->Start(0);
	if (findingIndex == 0)
		open.Add(start);
	while (open.Num() > 0)
	{
		auto currentNode = GetMinCostNode(open);
		currentNode->Cost = 0.0f;
		currentNode->State = EState::Closed;
		close.Add(currentNode);
		open.Remove(currentNode);
		if (currentNode->ID == end->ID)
		{
			findingIndex = 0;
			return EPathFindingResultState::Success;
		}
		for (auto neighbor : currentNode->NeighborWaypoints)
		{
			if (close.Contains(neighbor))
				continue;
			auto startToCurrent = (start->GetComponentLocation() - currentNode->GetComponentLocation()).Size();
			auto endToNeighbor = (end->GetComponentLocation() - neighbor->GetComponentLocation()).Size();
			auto neighborToCurrent = (neighbor->GetComponentLocation() - currentNode->GetComponentLocation()).Size();
			auto cost = startToCurrent + endToNeighbor + neighborToCurrent;
			if (cost < neighbor->Cost || neighbor->Cost == 0.0f)
			{
				neighbor->Cost = cost;
				neighbor->ParentWaypoint = currentNode;
			}
			if (!open.Contains(neighbor))
			{
				neighbor->State = EState::Open;
				open.Add(neighbor);
			}
		}
		findingIndex++;
		if (timer->GetElapsedTimeFromStart(0) > MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}

	findingIndex = 0;
	return EPathFindingResultState::Failed;
}

EPathFindingResultState UPathFindingComponent::ConvertRouteToVector(UWaypointComponent* start, UWaypointComponent* end, FVector endLoc, 
	TArray<UWaypointComponent*>&open, TArray<UWaypointComponent*>& close, TArray<FVector>& route)
{
	static auto currentNode = end;
	timer->Start(0);
	if (findingIndex == 0)
	{
		route.Add(endLoc);
		route.Add(end->GetComponentLocation());
		currentNode = end;
	}
	while (currentNode->ID != start->ID)
	{
		currentNode = currentNode->ParentWaypoint;
		route.Add(currentNode->GetComponentLocation());
		findingIndex++;
		if (timer->GetElapsedTimeFromStart(0) > MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}
	for (auto node : open)
	{
		node->State = EState::None;
		node->Cost = 0.0f;
		node->ParentWaypoint = nullptr;
	}
	for (auto node : close)
	{
		node->State = EState::None;
		node->Cost = 0.0f;
		node->ParentWaypoint = nullptr;
	}

	findingIndex = 0;
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
