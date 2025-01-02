#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoliagePlacer.generated.h"

class UFoliageType;

UCLASS()
class MYPROJECT_API AFoliagePlacer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFoliagePlacer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to place foliage
	UFUNCTION(BlueprintCallable, Category = "Foliage")
	void PlaceFoliage(UFoliageType* FoliageType, const TArray<FVector>& Locations);

private:
	void AddFoliageInstances(UWorld* World, UFoliageType* FoliageType, const TArray<FVector>& Locations);
};
