// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "ConvertingResultPath.h"
#include "WaypointComponent.h"
#include "TimeCounterComponent.h"
#include "PathFindingComponent.h"
#include "Engine/World.h"

EPathFindingState UConvertingResultPath::FindPath(UWorld* world, FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute)
{
	currentNode = pathFindInfo.EndNode;
	pathFindInfo.Timer->Start(0);
	if (index == 0)
	{
		resultRoute.Add(pathFindInfo.EndLocation);
		resultRoute.Add(pathFindInfo.EndNode->GetComponentLocation());
		currentNode = pathFindInfo.EndNode;
	}
	while (currentNode->ID != pathFindInfo.StartNode->ID)
	{
		currentNode = currentNode->ParentWaypoint;
		resultRoute.Add(currentNode->GetComponentLocation());
		index++;
		if (pathFindInfo.Timer->GetElapsedTimeFromStart(0) > pathFindInfo.MaxCaluclationTime)
			return EPathFindingState::ConvertingPathToVector;
	}
	for (auto node : pathFindInfo.OpenList)
	{
		node->State = EState::None;
		node->Cost = 0.0f;
		node->ParentWaypoint = nullptr;
	}
	for (auto node : pathFindInfo.CloseList)
	{
		node->State = EState::None;
		node->Cost = 0.0f;
		node->ParentWaypoint = nullptr;
	}
	auto keyString = FString::FromInt(pathFindInfo.StartNode->ID) + FString("_") + FString::FromInt(pathFindInfo.EndNode->ID);
	if (!pathFindInfo.RouteData.Contains(keyString))
		pathFindInfo.RouteData.Add(keyString, resultRoute);
	return EPathFindingState::Finished;
}
