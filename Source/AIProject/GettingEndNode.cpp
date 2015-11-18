// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "GettingEndNode.h"
#include "PathFindingComponent.h"
#include "TimeCounterComponent.h"
#include "WaypointComponent.h"
#include "Engine/World.h"

EPathFindingState UGettingEndNode::FindPath(UWorld* world, FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute)
{
	if (pathFindInfo.WaypointList.Num() == 0)
		return EPathFindingState::None;
	if (index == 0)
	{
		pathFindInfo.EndNode = pathFindInfo.WaypointList[0];
		minDistance = (pathFindInfo.EndNode->GetComponentLocation() - pathFindInfo.EndLocation).Size();
		index++;
	}
	while (index < pathFindInfo.WaypointList.Num())
	{
		auto currentNode = pathFindInfo.WaypointList[index];
		auto currentDistance = (currentNode->GetComponentLocation() - pathFindInfo.EndLocation).Size();
		if (currentDistance < minDistance)
		{
			if (!world->LineTraceTestByObjectType(pathFindInfo.EndLocation, currentNode->GetComponentLocation(), FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)))
			{
				pathFindInfo.EndNode = currentNode;
				minDistance = currentDistance;
			}
		}
		index++;
		if (pathFindInfo.Timer->GetElapsedTimeFromStart(0) >= pathFindInfo.MaxCaluclationTime)
			return EPathFindingState::GettingEndNode;
	}
	return EPathFindingState::LoadingFromDataMap;
}
