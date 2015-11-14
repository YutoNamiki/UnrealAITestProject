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
	if (GetWorld()->LineTraceTestByObjectType(startLocation, endLocation, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)))
		return EPathFindingResultState::Thinking;
	resultRoute.Empty();
	resultRoute.Add(endLocation);
	return EPathFindingResultState::Success;
}

EPathFindingResultState UPathFindingComponent::GetStartNode()
{
	if (Waypoints.Num() == 0)
		return EPathFindingResultState::Failed;
	static auto minDistance = 0.0f;
	timer->Start(0);
	if (findingIndex == 0)
	{
		startNode = Waypoints[0];
		minDistance = (startNode->GetComponentLocation() - startLocation).Size();
		findingIndex = 1;
	}
	while (findingIndex < Waypoints.Num())
	{
		auto currentNode = Waypoints[findingIndex];
		auto currentDistance = (currentNode->GetComponentLocation() - startLocation).Size();
		if (currentDistance < minDistance)
		{
			startNode = currentNode;
			minDistance = currentDistance;
		}
		findingIndex++;

		if (timer->GetElapsedTimeFromStart(0) >= MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}
	findingIndex = 0;
	return EPathFindingResultState::Success;
}

EPathFindingResultState UPathFindingComponent::GetEndNode()
{
	if (Waypoints.Num() == 0)
		return EPathFindingResultState::Failed;
	static auto minDistance = 0.0f;
	timer->Start(0);
	if (findingIndex == 0)
	{
		endNode = Waypoints[0];
		minDistance = (endNode->GetComponentLocation() - endLocation).Size();
		findingIndex = 1;
	}
	while (findingIndex < Waypoints.Num())
	{
		auto currentNode = Waypoints[findingIndex];
		auto currentDistance = (currentNode->GetComponentLocation() - endLocation).Size();
		if (currentDistance < minDistance)
		{
			endNode = currentNode;
			minDistance = currentDistance;
		}
		findingIndex++;

		if (timer->GetElapsedTimeFromStart(0) >= MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}
	findingIndex = 0;
	return EPathFindingResultState::Success;
}

EPathFindingResultState UPathFindingComponent::FindPathByAStarAlgorithm()
{
	timer->Start(0);
	if (findingIndex == 0)
		openList.Add(startNode);
	while (openList.Num() > 0)
	{
		auto currentNode = GetMinCostNode(openList);
		currentNode->Cost = 0.0f;
		currentNode->State = EState::Closed;
		closeList.Add(currentNode);
		openList.Remove(currentNode);
		if (currentNode->ID == endNode->ID)
		{
			findingIndex = 0;
			return EPathFindingResultState::Success;
		}
		for (auto neighbor : currentNode->NeighborWaypoints)
		{
			if (closeList.Contains(neighbor))
				continue;
			auto startToCurrent = (startNode->GetComponentLocation() - currentNode->GetComponentLocation()).Size();
			auto endToNeighbor = (endNode->GetComponentLocation() - neighbor->GetComponentLocation()).Size();
			auto neighborToCurrent = (neighbor->GetComponentLocation() - currentNode->GetComponentLocation()).Size();
			auto cost = startToCurrent + endToNeighbor + neighborToCurrent;
			if (cost < neighbor->Cost || neighbor->Cost == 0.0f)
			{
				neighbor->Cost = cost;
				neighbor->ParentWaypoint = currentNode;
			}
			if (!openList.Contains(neighbor))
			{
				neighbor->State = EState::Open;
				openList.Add(neighbor);
			}
		}
		findingIndex++;
		if (timer->GetElapsedTimeFromStart(0) > MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}

	findingIndex = 0;
	return EPathFindingResultState::Failed;
}

EPathFindingResultState UPathFindingComponent::ConvertRouteToVector(TArray<FVector>& route)
{
	static auto currentNode = endNode;
	timer->Start(0);
	if (findingIndex == 0)
	{
		route.Add(endLocation);
		route.Add(endNode->GetComponentLocation());
		currentNode = endNode;
	}
	while (currentNode->ID != startNode->ID)
	{
		currentNode = currentNode->ParentWaypoint;
		route.Add(currentNode->GetComponentLocation());
		findingIndex++;
		if (timer->GetElapsedTimeFromStart(0) > MaxCalculationTime)
			return EPathFindingResultState::Thinking;
	}
	for (auto node : openList)
	{
		node->State = EState::None;
		node->Cost = 0.0f;
		node->ParentWaypoint = nullptr;
	}
	for (auto node : closeList)
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
