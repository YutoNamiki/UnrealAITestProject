// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "FindingPathByAStarAlgorithm.h"
#include "WaypointComponent.h"
#include "TimeCounterComponent.h"
#include "PathFindingComponent.h"

EPathFindingState UFindingPathByAStarAlgorithm::FindPath(FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute)
{
	if (index == 0)
		pathFindInfo.OpenList.Add(pathFindInfo.StartNode);
	while (pathFindInfo.OpenList.Num() > 0)
	{
		auto currentNode = GetMinCostNode(pathFindInfo.OpenList);
		currentNode->Cost = 0.0f;
		currentNode->State = EState::Closed;
		pathFindInfo.CloseList.Add(currentNode);
		pathFindInfo.OpenList.Remove(currentNode);
		if (currentNode->ID == pathFindInfo.EndNode->ID)
		{
			index = 0;
			return EPathFindingState::ConvertingPathToVector;
		}
		for (auto neighbor : currentNode->NeighborWaypoints)
		{
			if (pathFindInfo.CloseList.Contains(neighbor))
				continue;
			auto startToCurrent = (pathFindInfo.StartNode->GetComponentLocation() - currentNode->GetComponentLocation()).Size();
			auto endToNeighbor = (pathFindInfo.EndNode->GetComponentLocation() - neighbor->GetComponentLocation()).Size();
			auto neighborToCurrent = (neighbor->GetComponentLocation() - currentNode->GetComponentLocation()).Size();
			auto cost = startToCurrent + endToNeighbor + neighborToCurrent;
			if (cost < neighbor->Cost || neighbor->Cost == 0.0f)
			{
				neighbor->Cost = cost;
				neighbor->ParentWaypoint = currentNode;
			}
			if (!pathFindInfo.OpenList.Contains(neighbor))
			{
				neighbor->State = EState::Open;
				pathFindInfo.OpenList.Add(neighbor);
			}
		}
		pathFindInfo.FindingIndex++;
		if (pathFindInfo.Timer->GetElapsedTimeFromStart(0) > pathFindInfo.MaxCaluclationTime)
			return EPathFindingState::PathFinding;
	}
	return EPathFindingState::None;
}

UWaypointComponent* UFindingPathByAStarAlgorithm::GetMinCostNode(TArray<UWaypointComponent*>& waypointList)
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
