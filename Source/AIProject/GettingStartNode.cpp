// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "GettingStartNode.h"
#include "PathFindingComponent.h"
#include "WaypointComponent.h"
#include "TimeCounterComponent.h"

EPathFindingState UGettingStartNode::FindPath(FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute)
{
	if (pathFindInfo.WaypointList.Num() == 0)
		return EPathFindingState::None;
	if (index == 0)
	{
		pathFindInfo.StartNode = pathFindInfo.WaypointList[0];
		minDistance = (pathFindInfo.StartNode->GetComponentLocation() - pathFindInfo.StartLocation).Size();
		index++;
	}
	while (index < pathFindInfo.WaypointList.Num())
	{
		auto currentNode = pathFindInfo.WaypointList[index];
		auto currentDistance = (currentNode->GetComponentLocation() - pathFindInfo.StartLocation).Size();
		if (currentDistance < minDistance)
		{
			if (!GetWorld()->LineTraceTestByObjectType(pathFindInfo.StartLocation, currentNode->GetComponentLocation(), FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic)))
			{
				pathFindInfo.StartNode = currentNode;
				minDistance = currentDistance;
			}
		}
		index++;
		if (pathFindInfo.Timer->GetElapsedTimeFromStart(0) >= pathFindInfo.MaxCaluclationTime)
			return EPathFindingState::GettingStartNode;
	}
	return EPathFindingState::GettingEndNode;
}
