// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "LineTracingForStartAndEnd.h"
#include "PathFindingComponent.h"
#include "Engine/World.h"

EPathFindingState ULineTracingForStartAndEnd::FindPath(UWorld* world, FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute)
{
	if (world->LineTraceTestByChannel(pathFindInfo.StartLocation, pathFindInfo.EndLocation, ECollisionChannel::ECC_WorldStatic))
		return EPathFindingState::GettingStartNode;
	resultRoute.Empty();
	resultRoute.Add(pathFindInfo.EndLocation);
	return EPathFindingState::Finished;
}
