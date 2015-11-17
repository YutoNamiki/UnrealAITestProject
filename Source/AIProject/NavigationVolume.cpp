// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "NavigationVolume.h"
#include "WaypointComponent.h"
#include "WaypointPathComponent.h"
#include "TimeCounterComponent.h"
#include "PathFindingComponent.h"

// Sets default values
ANavigationVolume::ANavigationVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxVolume = CreateDefaultSubobject<UBoxComponent>(FName("Volume"));
	RootComponent = BoxVolume;
	PathFindingComponent = CreateDefaultSubobject<UPathFindingComponent>(FName("PathFinding"));

	recursionIndex = 0;
}

// Called when the game starts or when spawned
void ANavigationVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavigationVolume::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

#if WITH_EDITOR
void  ANavigationVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Initialize();
}

void ANavigationVolume::EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);

	Initialize();
}

#endif

EPathFindingResultState ANavigationVolume::FindPathByVectors(APawn* findPawn, TArray<FVector>& resultRoute, FVector start, FVector end)
{
	return PathFindingComponent->FindPath(findPawn, resultRoute, start, end);
}

EPathFindingResultState ANavigationVolume::FindPathByActors(APawn* findPawn, TArray<FVector>& resultRoute, AActor* start, AActor* end)
{
	return PathFindingComponent->FindPath(findPawn, resultRoute, start->GetActorLocation(), end->GetActorLocation());
}

EPathFindingResultState ANavigationVolume::FindPathBySceneComponent(APawn* findPawn, TArray<FVector>& resultRoute, USceneComponent* start, USceneComponent* end)
{
	return PathFindingComponent->FindPath(findPawn, resultRoute, start->GetComponentLocation(), end->GetComponentLocation());
}

void ANavigationVolume::DrawPathFromVector(FVector start, TArray<FVector> route, FColor pathColor, float duration, float pathThickness)
{
	PathFindingComponent->DrawPath(start, route, pathColor, duration, pathThickness);
}

void ANavigationVolume::DrawPathFromActor(AActor* start, TArray<FVector> route, FColor pathColor, float duration, float pathThickness)
{
	PathFindingComponent->DrawPath(start->GetActorLocation(), route, pathColor, duration, pathThickness);
}

void ANavigationVolume::DrawPathFromSceneComponent(USceneComponent* start, TArray<FVector> route, FColor pathColor, float duration, float pathThickness)
{
	PathFindingComponent->DrawPath(start->GetComponentLocation(), route, pathColor, duration, pathThickness);
}

void ANavigationVolume::Initialize()
{
	WaypointList.Empty();
	WaypointPathList.Empty();
	DestroyChildrenComponents(BoxVolume);
	DivideVolume(BoxVolume, DivideX, DivideY, DivideZ);
	auto components = GetComponentsByClass(UWaypointComponent::StaticClass());
	for (auto component : components)
	{
		auto waypoint = Cast<UWaypointComponent>(component);
		if (waypoint == nullptr)
			continue;
		CreateOctree(waypoint, Recursion, recursionIndex, BoxVolume);
	}
	components = GetComponentsByClass(UWaypointComponent::StaticClass());
	auto num = 0;
	for (auto component : components)
	{
		auto waypoint = Cast<UWaypointComponent>(component);
		if (waypoint == nullptr)
			continue;
		waypoint->ID = num++;
		WaypointList.Add(waypoint);
	}
	CreatePaths(WaypointList, WaypointPathList, DrawPathColor, Thickness);
	PathFindingComponent->PathFindInfo.WaypointList.Empty();
	PathFindingComponent->PathFindInfo.WaypointList = WaypointList;
	WaypointCount = WaypointList.Num();
	WaypointPathCount = WaypointPathList.Num();
	DebugSettings(WaypointList, WaypointPathList, IsUseWaypointCollisions, IsVisiblePaths);
}

void ANavigationVolume::DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ)
{
	const auto maxPosition = volume->GetUnscaledBoxExtent() * 1.0f;
	const auto minPosition = volume->GetUnscaledBoxExtent() * -1.0f;
	const auto divideScale = FVector(static_cast<float>(divX), static_cast<float>(divY), static_cast<float>(divZ));
	const auto deltaVector = (maxPosition - minPosition) / divideScale;
	const auto initPosition = minPosition + deltaVector * 0.5f;
	auto num = 0;

	for (auto x = initPosition.X; x < maxPosition.X; x += deltaVector.X)
	{
		for (auto y = initPosition.Y; y < maxPosition.Y; y += deltaVector.Y)
		{
			for (auto z = initPosition.Z; z < maxPosition.Z; z += deltaVector.Z)
			{
				auto name = FString("Waypoint_") + FString::FromInt(num++);
				CreateWaypoint(FVector(x, y, z), deltaVector * 0.5f, name, volume);
			}
		}
	}
}

UWaypointComponent* ANavigationVolume::CreateWaypoint(FVector location, FVector extent, FString name, USceneComponent* inParent)
{
	auto waypoint = NewObject<UWaypointComponent>(this, FName(*name));
	waypoint->SetRelativeLocation(location);
	waypoint->ComponentTags.Add(FName("Waypoint"));
	if (inParent != nullptr)
		waypoint->AttachTo(inParent);
	waypoint->RegisterComponent();
	const auto waypointCollisionName = FName(*(name.Append(TEXT("_Collision"))));
	auto waypointCollision = NewObject<UBoxComponent>(this, waypointCollisionName);
	waypointCollision->ComponentTags.Add(FName("Waypoint"));
	waypointCollision->SetBoxExtent(extent);
	waypointCollision->SetCollisionProfileName(FName("OverlapAll"));
	waypointCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	waypointCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (inParent != nullptr)
		waypointCollision->AttachTo(waypoint);
	waypointCollision->RegisterComponent();
	
	return waypoint;
}

void ANavigationVolume::DestroyChildrenComponents(USceneComponent* component)
{
	while (component->GetNumChildrenComponents() > 0)
	{
		const auto childComponent = component->GetChildComponent(0);
		if (childComponent == nullptr)
			continue;
		DestroyChildrenComponents(childComponent);
		if (childComponent->IsRegistered())
			childComponent->UnregisterComponent();
		childComponent->DestroyComponent(false);
	}
}

void ANavigationVolume::CreateOctree(UWaypointComponent* waypoint, int32 recursion, int32 recursionIndex, USceneComponent* inParent)
{
	if (!waypoint->HasChildBoxCollision())
	{
		UE_LOG(LogTemp, Warning, TEXT("Waypoint Has Not Child Box Collision."));
		return;
	}
	const auto boxCollision = waypoint->GetChildBoxCollision();
	const auto localLocation = waypoint->GetRelativeTransform().GetLocation();
	const auto collisionExtent = boxCollision->GetScaledBoxExtent();
	const auto localExtent = collisionExtent / 2.0f;
	const auto localOffset = collisionExtent / 2.0f;
	const auto waypointName = waypoint->GetName();

	TArray<FOverlapResult> overlapResults;
	if (!GetWorld()->OverlapMultiByChannel(overlapResults, boxCollision->GetComponentLocation(), boxCollision->GetComponentQuat(), 
		ECollisionChannel::ECC_WorldStatic, FCollisionShape::MakeBox(collisionExtent)))
		return;
	if (boxCollision->IsRegistered())
		boxCollision->UnregisterComponent();
	boxCollision->DestroyComponent();
	if (waypoint->IsRegistered())
		waypoint->UnregisterComponent();
	waypoint->DestroyComponent();
	if (recursionIndex >= recursion)
		return;
	recursionIndex++;
	
	CreateOctree(CreateWaypoint(FVector(localLocation.X + localOffset.X, localLocation.Y + localOffset.Y, localLocation.Z + localOffset.Z), localExtent, waypointName + "_1", inParent), 
		recursion, recursionIndex, inParent);
	CreateOctree(CreateWaypoint(FVector(localLocation.X - localOffset.X, localLocation.Y + localOffset.Y, localLocation.Z + localOffset.Z), localExtent, waypointName + "_2", inParent),
		recursion, recursionIndex, inParent);
	CreateOctree(CreateWaypoint(FVector(localLocation.X + localOffset.X, localLocation.Y - localOffset.Y, localLocation.Z + localOffset.Z), localExtent, waypointName + "_3", inParent),
		recursion, recursionIndex, inParent);
	CreateOctree(CreateWaypoint(FVector(localLocation.X + localOffset.X, localLocation.Y + localOffset.Y, localLocation.Z - localOffset.Z), localExtent, waypointName + "_4", inParent),
		recursion, recursionIndex, inParent);
	CreateOctree(CreateWaypoint(FVector(localLocation.X + localOffset.X, localLocation.Y - localOffset.Y, localLocation.Z - localOffset.Z), localExtent, waypointName + "_5", inParent),
		recursion, recursionIndex, inParent);
	CreateOctree(CreateWaypoint(FVector(localLocation.X - localOffset.X, localLocation.Y - localOffset.Y, localLocation.Z + localOffset.Z), localExtent, waypointName + "_6", inParent),
		recursion, recursionIndex, inParent);
	CreateOctree(CreateWaypoint(FVector(localLocation.X - localOffset.X, localLocation.Y + localOffset.Y, localLocation.Z - localOffset.Z), localExtent, waypointName + "_7", inParent),
		recursion, recursionIndex, inParent);
	CreateOctree(CreateWaypoint(FVector(localLocation.X - localOffset.X, localLocation.Y - localOffset.Y, localLocation.Z - localOffset.Z), localExtent, waypointName + "_8", inParent),
		recursion, recursionIndex, inParent);
}

void ANavigationVolume::CreatePaths(const TArray<UWaypointComponent*>& waypointList, TArray<UWaypointPathComponent*>& waypointPathList, FColor color, float thickness)
{
	TArray<FOverlapResult> overlapResults;
	TArray<USceneComponent*> parentComponents;
	int32 id = 0;

	for (auto waypoint : waypointList)
	{
		overlapResults.Empty();
		auto collision = waypoint->GetChildBoxCollision();
		if (!GetWorld()->ComponentOverlapMulti(overlapResults, collision, collision->GetComponentLocation(), collision->GetComponentRotation()))
			continue;
		for (auto overlapResult : overlapResults)
		{
			auto waypointCollisionComponent = Cast<UBoxComponent>(overlapResult.GetComponent());
			if (waypointCollisionComponent == nullptr)
				continue;
			if (!waypointCollisionComponent->ComponentHasTag(FName("Waypoint")))
				continue;
			parentComponents.Empty();
			waypointCollisionComponent->GetParentComponents(parentComponents);
			for (auto parentComponent : parentComponents)
			{
				auto waypointComponent = Cast<UWaypointComponent>(parentComponent);
				if (waypointComponent == nullptr)
					continue;
				if (!waypointComponent->ComponentHasTag(FName("Waypoint")))
					continue;
				waypoint->NeighborWaypoints.Add(waypointComponent);
				waypointPathList.Add(CreateWaypointPath(waypoint, waypointComponent, FString("WaypointPath_") + FString::FromInt(id++), color, thickness));
			}
		}
	}
	for (auto index1 = 0; index1 < waypointPathList.Num() - 1; index1++)
	{
		for (auto index2 = index1 + 1; index2 < waypointPathList.Num(); index2++)
		{
			auto path1 = waypointPathList[index1];
			auto path2 = waypointPathList[index2];
			if (path1->Waypoint1->ID == path2->Waypoint2->ID && path1->Waypoint2->ID == path2->Waypoint1->ID)
				waypointPathList.RemoveAt(index2--);
		}
	}
}

UWaypointPathComponent* ANavigationVolume::CreateWaypointPath(UWaypointComponent* waypoint1, UWaypointComponent* waypoint2, FString pathName, FColor color, float thickness)
{
	auto* path = NewObject<UWaypointPathComponent>(this, FName(*pathName));
	path->Waypoint1 = waypoint1;
	path->Waypoint2 = waypoint2;
	path->DrawPathColor = color;
	path->Thickness = thickness;
	return path;
}

void ANavigationVolume::DebugSettings(TArray<UWaypointComponent*>& waypointList, TArray<UWaypointPathComponent*>& waypointPathList, bool isUseWaypointCollisions, bool isVisiblePaths)
{
	if (!isUseWaypointCollisions)
	{
		for (auto waypoint : waypointList)
			DestroyChildrenComponents(waypoint);
	}
	if (!isVisiblePaths)
	{
		for (auto path : waypointPathList)
			path->DestroyComponent();
		waypointPathList.Empty();
	}
}
