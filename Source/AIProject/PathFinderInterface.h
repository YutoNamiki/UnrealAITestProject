// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PathFinderInterface.generated.h"

class UWorld;
struct FPathFindingInformation;

UENUM(BlueprintType)
enum class EPathFindingState : uint8
{
	None					UMETA(DisplayName = "None"),
	LineTracing				UMETA(DisplayName = "LineTrace"),
	GettingStartNode		UMETA(DisplayName = "GettingStartNode"),
	GettingEndNode			UMETA(DisplayName = "GettingEndNode"),
	LoadingFromDataMap		UMETA(DisplayName = "LoadingFromDataMap"),
	PathFinding				UMETA(DisplayName = "PathFinding"),
	ConvertingPathToVector	UMETA(DisplayName = "ConvertingPathToVector"),
	Finished				UMETA(DisplayName = "Finished")
};

UENUM(BlueprintType)
enum class EPathFindingResultState : uint8
{
	Failed		UMETA(DisplayName = "Failed"),
	Thinking	UMETA(DisplayName = "Thinking"),
	Success		UMETA(DisplayName = "Success")
};

UINTERFACE(MinimalAPI)
class UPathFinderInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IPathFinderInterface : public IInterface
{
	GENERATED_IINTERFACE_BODY()

	virtual EPathFindingState FindPath(UWorld* world, FPathFindingInformation& pathFindInfo, TArray<FVector>& resultRoute) = 0;
};
