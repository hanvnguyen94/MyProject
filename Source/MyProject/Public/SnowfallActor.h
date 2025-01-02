// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "SnowfallActor.generated.h"

USTRUCT(BlueprintType)
struct FParticle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	float Diameter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
	float Speed;
};

UCLASS()
class MYPROJECT_API ASnowfallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnowfallActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	// Class variables for simulation
	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float Alpha; // Shape parameter for gamma distribution

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float Beta; // Scale parameter for gamma distribution

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	int32 NumParticles; // Number of snowflakes

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float SpawnHeight; // Height of the spawn area

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	float ScreenWidth; // Width of the spawn area

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	FVector2D WindSpeed; // X: horizontal wind speed, Y: vertical wind offset (if needed)

	UPROPERTY(EditAnywhere, Category = "Snowfall")
	UStaticMeshComponent* GroundMesh;

	TArray<FParticle> Particles; // Array of particles

	UPROPERTY(EditAnywhere, Category = "Niagara")
	UNiagaraSystem* SnowNiagaraSystem; // Reference to Niagara system

	UPROPERTY()
	UNiagaraComponent* NiagaraComponent; // Niagara component for spawning particles

	// Functions for particle initialization and update
	void InitializeParticles();
	void SpawnParticle(FParticle& Particle);
	void UpdateParticle(FParticle& Particle, float DeltaTime);
	float GenerateParticleDiameter();
	float CalculateTerminalSpeed(float Diameter);
	void UpdateNiagaraParameters();
	FVector2D PredictParticlePosition(FParticle& Particle, float Time);
};
