// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldGenerator.generated.h"

UCLASS()
class MYPROJECT_API AWorldGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int X_Vertex = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Y_Vertex = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CellSize = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumOfSectionX = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int NumOfSectionY = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MeshSectionIndex = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
