// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject.h"
#include "MyTestActor.h"


// Sets default values
AMyTestActor::AMyTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->OnActorBeginOverlap.AddDynamic(this, &AMyTestActor::OnTestActorBeginOverlap);
	this->OnActorEndOverlap.AddDynamic(this, &AMyTestActor::OnTestActorEndOverlap);
	this->OnActorHit.AddDynamic(this, &AMyTestActor::OnTestActorHit);
	this->OnBeginCursorOver.AddDynamic(this, &AMyTestActor::OnTestBeginCursorOver);
	this->OnClicked.AddDynamic(this, &AMyTestActor::OnTestClicked);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(FName("Volume"));
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = BoxCollision;
	BoxCollision->OnClicked.AddDynamic(this, &AMyTestActor::OnUPrimitiveComponentClicked);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyTestActor::OnUPrimitiveComponentBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AMyTestActor::OnUPrimitiveComponentEndOverlap);
}

// Called when the game starts or when spawned
void AMyTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyTestActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMyTestActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("UObject:PostEditChangeProperty"));
	Super::PostEditChangeProperty(PropertyChangeEvent);
}

void AMyTestActor::PostEditImport()
{
	UE_LOG(LogTemp, Warning, TEXT("UObject:PostEditImport"));
	Super::PostEditImport();
}

void AMyTestActor::PostEditUndo(TSharedPtr<ITransactionObjectAnnotation> TransactionAnnotation)
{
	UE_LOG(LogTemp, Warning, TEXT("UObject:PostEditUndo"));
	Super::PostEditUndo(TransactionAnnotation);
}

void AMyTestActor::PostInitProperties()
{
	UE_LOG(LogTemp, Warning, TEXT("UObject:PostEditProperties"));
	Super::PostInitProperties();
}

void AMyTestActor::PostLoad()
{
	UE_LOG(LogTemp, Warning, TEXT("UObject:PostLoad"));
	Super::PostLoad();
}

void AMyTestActor::PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph)
{
	UE_LOG(LogTemp, Warning, TEXT("UObject:PostLoadSubobjects"));
	Super::PostLoadSubobjects(OuterInstanceGraph);
}

void AMyTestActor::PostNetReceive()
{
	UE_LOG(LogTemp, Warning, TEXT("UObject:PostNetReceive"));
	Super::PostNetReceive();
}

void AMyTestActor::PreEditChange(UProperty* PropertyAbountToChange)
{
	UE_LOG(LogTemp, Warning, TEXT("UObject:PreEditChange"));
	Super::PreEditChange(PropertyAbountToChange);
}

void AMyTestActor::PreEditUndo()
{
	UE_LOG(LogTemp, Warning, TEXT("UObject:PreEditUndo"));
	Super::PreEditUndo();
}

void AMyTestActor::PreNetReceive()
{
	UE_LOG(LogTemp, Warning, TEXT("UObject:PreNetReceive"));
	Super::PreNetReceive();
}

void AMyTestActor::PostEditUndo()
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:PostEditUndo"));
	Super::PostEditUndo();
}

void AMyTestActor::EditorApplyMirror(const FVector& MirrorScale, const FVector& PivotLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:EditorApplyMirror"));
	Super::EditorApplyMirror(MirrorScale, PivotLocation);
}

void AMyTestActor::EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:EditorApplyRotation"));
	Super::EditorApplyRotation(DeltaRotation, bAltDown, bShiftDown, bCtrlDown);
}

void AMyTestActor::EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:EditorApplyScale"));
	Super::EditorApplyScale(DeltaScale, PivotLocation, bAltDown, bShiftDown, bCtrlDown);
}

void AMyTestActor::EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:EditorApplyTranslation"));
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);

	TestFunction();
}

void AMyTestActor::EditorKeyPressed(FKey Key, EInputEvent Event)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:EditorKeyPressed"));
	Super::EditorKeyPressed(Key, Event);
}

void AMyTestActor::EditorReplacedActor(AActor* OldActor)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:EditorReplacedActor"));
	Super::EditorReplacedActor(OldActor);
}

void AMyTestActor::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:OnConstruction"));
	Super::OnConstruction(Transform);

	TestFunction();
}

void AMyTestActor::OnSerializeNewActor(FOutBunch& OutBunch)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:OnSerializeNewActor"));
	Super::OnSerializeNewActor(OutBunch);
}

void AMyTestActor::PostActorCreated()
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:PostActorCreated"));
	Super::PostActorCreated();
}

void AMyTestActor::PostEditMove(bool bFinished)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:PostEditMove"));
	Super::PostEditMove(bFinished);
}

void AMyTestActor::PostNetInit()
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:PostNetInit"));
	Super::PostNetInit();
}

void AMyTestActor::PostNetReceiveLocationAndRotation()
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:PostNetReceiveLocationAndRotation"));
	Super::PostNetReceiveLocationAndRotation();
}

void AMyTestActor::PostNetReceivePhysicState()
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:PostNetReceivePhysicState"));
	Super::PostNetReceivePhysicState();
}

void AMyTestActor::PostNetReceiveVelocity(const FVector& NewVelocity)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:PostNetReceiveVelocity"));
	Super::PostNetReceiveVelocity(NewVelocity);
}

void AMyTestActor::PostRegisterAllComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:PostRegisterAllComponents"));
	Super::PostRegisterAllComponents();
}

void AMyTestActor::PostUnregisterAllComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:PostUnregisterAllComponents"));
	Super::PostUnregisterAllComponents();
}

void AMyTestActor::PreInitializeComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:PreInitializeComponents"));
	Super::PreInitializeComponents();
}

void AMyTestActor::OnTestActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:OnActorBeginOverlap"));
}

void AMyTestActor::OnTestActorEndOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:OnActorEndOverlap"));
}

void AMyTestActor::OnTestActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:OnActorHit"));
}

void AMyTestActor::OnTestBeginCursorOver()
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:OnBeginCursorOver"));
}

void AMyTestActor::OnTestClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("AActor:OnClicked"));
}

void AMyTestActor::OnUPrimitiveComponentClicked(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor(Component):OnClicked"));
}

void AMyTestActor::OnUPrimitiveComponentBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OhterBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor(Component):OnComponentBeginOverlap"));
}

void AMyTestActor::OnUPrimitiveComponentEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OhterBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("AActor(Component):OnComponentEndOverlap"));
}

void AMyTestActor::TestFunction()
{
	TArray<FOverlapResult> overlapResults;
	//if (GetWorld()->OverlapMultiByObjectType(overlapResults, BoxCollision->GetComponentLocation(), BoxCollision->GetComponentQuat(),
	//	FCollisionObjectQueryParams(), FCollisionShape::MakeBox(BoxCollision->GetScaledBoxExtent())))
	if (GetWorld()->ComponentOverlapMulti(overlapResults, BoxCollision, BoxCollision->GetComponentLocation(), BoxCollision->GetComponentRotation()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping!"));
		UE_LOG(LogTemp, Warning, TEXT("Count: %d objects"), overlapResults.Num());
		for (auto overlapResult : overlapResults)
		{
			auto actor = overlapResult.GetActor();
			auto component = overlapResult.GetComponent();
			auto messageText = FString("Actor: ") + actor->GetName() + FString(" Component: ") + component->GetName();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *messageText);
		}
	}
}
