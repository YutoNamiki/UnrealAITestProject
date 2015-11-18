// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "LoadingFromDataMap.h"
#include "WaypointComponent.h"
#include "PathFindingComponent.h"
#include "Engine/World.h"

EPathFindingState ULoadingFromDataMap::FindPath(UWorld* world, FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute)
{
	auto keyString = FString::FromInt(pathFindInfo.StartNode->ID) + FString("_") + FString::FromInt(pathFindInfo.EndNode->ID);
	if (pathFindInfo.RouteData.Num() == 0)
		return EPathFindingState::PathFinding;
	if (!pathFindInfo.RouteData.Contains(keyString))
		return EPathFindingState::PathFinding;
	resultRoute = pathFindInfo.RouteData[keyString];
	return EPathFindingState::Finished;
}
