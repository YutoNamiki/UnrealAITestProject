// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "NavigationVolume.h"
#include "WaypointComponent.h"

// Sets default values
ANavigationVolume::ANavigationVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxVolume = CreateDefaultSubobject<UBoxComponent>(FName("Volume"));
	RootComponent = BoxVolume;

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

void ANavigationVolume::Initialize()
{
	WaypointList.Empty();
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
	auto i = 0;
	for (auto component : components)
	{
		auto waypoint = Cast<UWaypointComponent>(component);
		if (waypoint == nullptr)
			continue;
		waypoint->ID = i++;
		WaypointList.Add(waypoint);
	}
	WaypointCount = WaypointList.Num();

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
	auto* waypoint = NewObject<UWaypointComponent>(this, FName(*name));
	waypoint->SetRelativeLocation(location);
	waypoint->ComponentTags.Add(FName("Waypoint"));
	if (inParent != nullptr)
	{
		waypoint->AttachTo(inParent);
	}
	waypoint->RegisterComponent();
	auto waypointCollisionName = FName(*(name.Append(TEXT("_Collision"))));
	auto* waypointCollision = NewObject<UBoxComponent>(this, waypointCollisionName);
	waypointCollision->ComponentTags.Add(FName("Waypoint"));
	waypointCollision->SetBoxExtent(extent);
	waypointCollision->SetCollisionProfileName(FName("OverlapAll"));
	waypointCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	waypointCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (inParent != nullptr)
	{
		waypointCollision->AttachTo(waypoint);
	}
	waypointCollision->RegisterComponent();
	
	return waypoint;
}

void ANavigationVolume::DestroyChildrenComponents(USceneComponent* component)
{
	while (component->GetNumChildrenComponents() > 0)
	{
		auto childComponent = component->GetChildComponent(0);
		childComponent->UnregisterComponent();
		childComponent->DestroyComponent(true);
	}
}

void ANavigationVolume::CreateOctree(UWaypointComponent* waypoint, int32 recursion, int32 recursionIndex, USceneComponent* inParent)
{
	if (!waypoint->HasChildBoxCollision())
	{
		UE_LOG(LogTemp, Warning, TEXT("Waypoint Has Not Child Box Collision."));
		return;
	}

	auto boxCollision = waypoint->GetChildBoxCollision();
	auto localLocation = waypoint->GetRelativeTransform().GetLocation();
	auto collisionExtent = boxCollision->GetScaledBoxExtent();
	auto localExtent = collisionExtent / 2.0f;
	auto localOffset = collisionExtent / 2.0f;
	auto waypointName = waypoint->GetName();

	TArray<FOverlapResult> overlapResults;
	if (!GetWorld()->OverlapMultiByChannel(overlapResults, boxCollision->GetComponentLocation(), boxCollision->GetComponentQuat(), 
		ECollisionChannel::ECC_WorldStatic, FCollisionShape::MakeBox(collisionExtent)))
	{
		return;
	}

	boxCollision->UnregisterComponent();
	boxCollision->DestroyComponent();
	waypoint->UnregisterComponent();
	waypoint->DestroyComponent();

	if (recursionIndex >= recursion)
	{
		return;
	}
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

void ANavigationVolume::CreatePaths(const TArray<UWaypointComponent*>& waypointList)
{
	TArray<UPrimitiveComponent*> overlapResults;
	for (auto waypoint : waypointList)
	{
		overlapResults.Empty();
		auto collision = waypoint->GetChildBoxCollision();
		
	}
}
