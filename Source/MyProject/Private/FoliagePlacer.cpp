#include "MyProject/Public/FoliagePlacer.h"

#include "FoliageType.h"
#include "InstancedFoliageActor.h"

// Sets default values
AFoliagePlacer::AFoliagePlacer()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFoliagePlacer::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AFoliagePlacer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFoliagePlacer::PlaceFoliage(UFoliageType* FoliageType, const TArray<FVector>& Locations)
{
    if (!FoliageType)
    {
        UE_LOG(LogTemp, Warning, TEXT("FoliageType is null. Cannot place foliage."));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("World is null. Cannot place foliage."));
        return;
    }

    AddFoliageInstances(World, FoliageType, Locations);
}

void AFoliagePlacer::AddFoliageInstances(UWorld* World, UFoliageType* FoliageType, const TArray<FVector>& Locations)
{
    if (!World || !FoliageType)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid parameters for adding foliage instances."));
        return;
    }

    // Get the Instanced Foliage Actor in the current level
    AInstancedFoliageActor* FoliageActor = AInstancedFoliageActor::GetInstancedFoliageActorForCurrentLevel(World);
    if (!FoliageActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find Instanced Foliage Actor in the current level."));
        return;
    }

    // Retrieve or create foliage info for the specified foliage type
    FFoliageInfo* FoliageInfo = FoliageActor->FindOrAddMesh(FoliageType);
    if (!FoliageInfo)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to retrieve or create foliage info for the given foliage type."));
        return;
    }

    // Add foliage instances
    for (const FVector& Location : Locations)
    {
        FFoliageInstance FoliageInstance;
        FoliageInstance.Location = Location;
        FoliageInstance.Rotation = FRotator::ZeroRotator;
        FoliageInstance.DrawScale3D = FVector3f(1.0f, 1.0f, 1.0f); // Default scale

        // Add the instance to the foliage info
        FoliageInfo->AddInstance(FoliageType, FoliageInstance); // Pass FoliageType as the first parameter
    }

    UE_LOG(LogTemp, Log, TEXT("Successfully placed foliage at specified locations."));
}

void ExampleUsage(UWorld* World, AFoliagePlacer* FoliagePlacer)
{
    if (!FoliagePlacer) return;

    UFoliageType* FoliageType = LoadObject<UFoliageType>(nullptr, TEXT("/Game/Foliage/MyFoliageType.MyFoliageType"));
    if (!FoliageType)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load foliage type."));
        return;
    }

    TArray<FVector> Locations = {
        FVector(100.0f, 100.0f, 0.0f),
        FVector(200.0f, 100.0f, 0.0f),
        FVector(300.0f, 100.0f, 0.0f)
    };

    FoliagePlacer->PlaceFoliage(FoliageType, Locations);
}